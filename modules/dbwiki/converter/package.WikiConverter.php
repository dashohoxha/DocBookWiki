<?php
  /*
   This  file is part  of WikiConverter.   WikiConverter is  a program
   that  converts   text/wiki  into   other  formats  (like   html  or
   xml/docbook).

   Copyright (c) 2005 Dashamir Hoxha, dhoxha@inima.al

   WikiConverter  is free  software;  you can  redistribute it  and/or
   modify  it under the  terms of  the GNU  General Public  License as
   published by the Free Software  Foundation; either version 2 of the
   License, or (at your option) any later version.

   WikiConverter is  distributed in the  hope that it will  be useful,
   but  WITHOUT ANY  WARRANTY; without  even the  implied  warranty of
   MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
   General Public License for more details.

   You should have  received a copy of the  GNU General Public License
   along  with  WikiConverter; if  not,  write  to  the Free  Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA
  */

  /**
   * @package docbook
   * @subpackage wikiconverter
   */

$path = dirname(__FILE__).'/';
include_once $path.'class.Info.php';
include_once $path.'class.Bibliography.php';
include_once $path.'class.Tpl.php';
include_once $path.'class.Text.php';
include_once $path.'class.Paragraph.php';
include_once $path.'class.Block.php';
include_once $path.'class.Listing.php';
include_once $path.'class.ListItem.php';
include_once $path.'class.WikiParser.php';
include_once $path.'fun.regex_replace.php';

/** A collection of the templates that have been parsed. */
$tpl_collection = array();
?>