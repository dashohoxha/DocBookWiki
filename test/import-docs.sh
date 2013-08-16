#!/bin/bash

drush="drush --include=../modules/docbook/drush/"

cd $(dirname $0)

$drush docbook2dbw $(pwd)/slackrouter_en.xml > slackrouter_en_1.xml
$drush -u admin dbw-import $(pwd)/slackrouter_en_1.xml

#$drush docbook2dbw $(pwd)/gateway_server_en.xml > gateway_server_en_1.xml
#$drush -u admin dbw-import $(pwd)/gateway_server_en_1.xml

