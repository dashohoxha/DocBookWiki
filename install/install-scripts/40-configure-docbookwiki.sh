#!/bin/bash -x

# Protect Drupal settings from prying eyes
drupal_settings=/var/www/docbookwiki/sites/default/settings.php
chown root:www-data $drupal_settings
chmod 640 $drupal_settings

### Modify Drupal settings

# diable poor man's cron
cat >> $drupal_settings << EOF
/**
 * Disable Poor Man's Cron:
 *
 * Drupal 7 enables the built-in Poor Man's Cron by default.
 * Poor Man's Cron relies on site activity to trigger Drupal's cron,
 * and is not well suited for low activity websites.
 *
 * We will use the Linux system cron and override Poor Man's Cron
 * by setting the cron_safe_threshold to 0.
 *
 * To re-enable Poor Man's Cron:
 *    Comment out (add a leading hash sign) the line below,
 *    and the system cron in /etc/cron.d/drupal7.
 */
\$conf['cron_safe_threshold'] = 0;

EOF

# set base_url
cat >> $drupal_settings << EOF
\$base_url = "https://doc.example.org";

EOF

# set the memcache configuration
cat >> $drupal_settings << EOF
// Adds memcache as a cache backend
\$conf['cache_backends'][] = 'profiles/docbookwiki/modules/contrib/memcache/memcache.inc';
// Makes it so that memcache is the default caching backend
\$conf['cache_default_class'] = 'MemCacheDrupal';
// Keep forms in persistent storage, as per discussed at the beginning
\$conf['cache_class_cache_form'] = 'DrupalDatabaseCache';
// I don't see any point in keeping the module update information in Memcached
\$conf['cache_class_cache_update'] = 'DrupalDatabaseCache';

// Specify the memcache servers you wish to use and assign them to a cluster
// Cluster = group of memcache servers, in our case, it's probably just one server per cluster.
\$conf['memcache_servers'] = array('unix:///var/run/memcached/memcached.sock' => 'default');
// This assigns all cache bins to the 'default' cluster from above
\$conf['memcache_bins'] = array('cache' => 'default');

// If you wanted multiple Drupal installations to share one Memcache instance use the prefix like so:
\$conf['memcache_key_prefix'] = 'docbookwiki';

EOF

### update to the latest version of core and modules
drush --yes pm-update

### install features modules
drush --yes pm-enable docbookwiki_docbook
drush --yes features-revert docbookwiki_docbook

drush --yes pm-enable docbookwiki_layout
drush --yes features-revert docbookwiki_layout

#drush --yes pm-enable docbookwiki_misc
#drush --yes features-revert docbookwiki_misc

#drush --yes pm-enable docbookwiki_disqus
#drush --yes pm-enable docbookwiki_content
#drush --yes pm-enable docbookwiki_sharethis

#drush --yes pm-enable docbookwiki_captcha
#drush --yes features-revert docbookwiki_captcha
#drush vset recaptcha_private_key 6LenROISAAAAAM-bbCjtdRMbNN02w368ScK3ShK0
#drush vset recaptcha_public_key 6LenROISAAAAAH9roYsyHLzGaDQr76lhDZcm92gG

#drush --yes pm-enable docbookwiki_invite
#drush --yes pm-enable docbookwiki_permissions

#drush --yes pm-enable docbookwiki_simplenews
#drush --yes pm-enable docbookwiki_mass_contact
#drush --yes pm-enable docbookwiki_googleanalytics
#drush --yes pm-enable docbookwiki_drupalchat

### install also multi-language support
#drush --yes pm-enable l10n_client l10n_update
#mkdir -p /var/www/docbookwiki/sites/all/translations
#chown -R www-data: /var/www/docbookwiki/sites/all/translations
#drush --yes l10n-update