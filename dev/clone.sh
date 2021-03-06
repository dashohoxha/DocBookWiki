#!/bin/bash
### Create a local clone of the main drupal
### application (/var/www/docbookwiki).

if [ $# -ne 1 ]
then
    echo " * Usage: $0 variant

      Creates a local clone of the main drupal application.
      <variant> can be something like 'dev', 'test', '01', etc.
      It will create a new application with root
      /var/www/docbookwiki_<variant> and with DB named
      docbookwiki_<variant>

      Caution: The root directory and the DB will be erased,
      if they exist.
"
    exit 1
fi
var=$1
root_dir=/var/www/docbookwiki_$var
db_name=docbookwiki_$var

### copy the root directory
rm -rf $root_dir
cp -a /var/www/docbookwiki $root_dir

### modify settings.php
domain=$(cat /etc/hostname)
sed -i $root_dir/sites/default/settings.php \
    -e "/^\\\$databases = array/,+10  s/'database' => .*/'database' => '$db_name',/" \
    -e "/^\\\$base_url/c \$base_url = \"https://$var.$domain\";" \
    -e "/^\\\$conf\['memcache_key_prefix'\]/c \$conf['memcache_key_prefix'] = 'docbookwiki_$var';"

### create a drush alias
sed -i /etc/drush/local.aliases.drushrc.php \
    -e "/^\\\$aliases\['$var'\] = /,+5 d"
cat <<EOF >> /etc/drush/local.aliases.drushrc.php
\$aliases['$var'] = array (
  'parent' => '@main',
  'root' => '/var/www/docbookwiki_$var',
  'uri' => 'http://$var.doc.example.org',
);

EOF

### create a new database
mysql --defaults-file=/etc/mysql/debian.cnf -e "
    DROP DATABASE IF EXISTS $db_name;
    CREATE DATABASE $db_name;
    GRANT ALL ON $db_name.* TO docbookwiki@localhost;
"

### copy the database
drush sql-sync @self @$var

### clear the cache
drush @$var cc all

### copy and modify the configuration of nginx
rm -f /etc/nginx/sites-{available,enabled}/$var
cp /etc/nginx/sites-available/{default,$var}
sed -i /etc/nginx/sites-available/$var \
    -e "s/443 default ssl/443 ssl/" \
    -e "s/server_name \(.*\);/server_name $var.\\1;/" \
    -e "s/docbookwiki/docbookwiki_$var/g"
ln -s /etc/nginx/sites-{available,enabled}/$var

### copy and modify the configuration of apache2
rm -f /etc/apache2/sites-{available,enabled}/$var{,-ssl}
cp /etc/apache2/sites-available/{default,$var}
cp /etc/apache2/sites-available/{default-ssl,$var-ssl}
sed -i /etc/apache2/sites-available/$var \
    -e "s/ServerName \(.*\)/ServerName $var.\\1/" \
    -e "s/docbookwiki/docbookwiki_$var/g"
sed -i /etc/apache2/sites-available/$var-ssl \
    -e "s/ServerName \(.*\)/ServerName $var.\\1/" \
    -e "s/docbookwiki/docbookwiki_$var/g"
a2ensite $var $var-ssl

### restart services
#for SRV in php5-fpm memcached mysql nginx
for SRV in mysql apache2
do
    service $SRV restart
done
