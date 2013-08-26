<?php
/**
 * @file
 * Theme related functions.
 */

/**
 * Implements hook_theme().
 */
function docbook_theme($existing, $type, $theme, $path) {
  return array (
    'node__docbook' => array(
      'template' => 'node--docbook',
    ),
    'node__docbook_section' => array(
      'template' => 'node--docbook-section' ,
    ),
    'field__dbw_author' => array(
    ),
  );
}

/**
 * Implements hook_preprocess_HOOK().
 */
function docbook_preprocess_node(&$variables) {
  //krumo($variables);  //debug

  if ($variables['type'] == 'docbook') {
    $variables['variable1'] = 'hello1';
  }

  if ($variables['type'] == 'docbook_section') {
    $variables['variable2'] = 'hello2';
  }
}

/**
 * Implementing theme function theme_HOOK().
 */
function theme_field__dbw_author($variables) {
  //krumo($variables);  //debug

  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div class="field-items"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<div class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . _docbook_render_author($item['#markup']) . '</div>';
  }
  $output .= '</div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

function _docbook_render_author($author) {
  list($firstname, $lastname, $email, $org, $org_url) = explode(',', $author, 5);
  $firstname = trim($firstname);
  $lastname = trim($lastname);
  $email = trim($email);
  $org = trim($org);
  $org_url = trim($org_url);

  if (empty($email)) {
    $name = "$firstname $lastname";
  }
  else {
    $name = "<a href='mailto:$email'>$firstname $lastname</a>";
  }

  if (!empty($org) and !empty($org_url)) {
    $org = "<a href='$org_url'>$org</a>";
  }

  if (empty($org)) {
    return $name;
  }
  else {
    return "$name, $org";
  }
}
