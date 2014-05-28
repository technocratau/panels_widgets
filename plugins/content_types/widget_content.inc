<?php

/**
 * @file
 * Content type plugin to allow Widgets to be viewed as content types.
 */

/**
 * Implements hook_ctools_content_types().
 */
function panels_widgets_widget_content_ctools_content_types() {
  return array(
    'title' => t('Panels Widgets'),
    'content type' => 'panels_widgets_widget_content_content_type_content_type',
  );
}

/**
 * Return all widget types available as a subtype.
 *
 * @param array $plugin
 *   No idea why this is added, coudn't find any documentation surrounding it.
 *
 * @return array
 *   A types array associating widget type name with plugin settings.
 *
 * @Todo: Implement cache checks.
 */
function panels_widgets_widget_content_content_type_content_types($plugin) {
  $types = array();

  // Load available Widget Types and loop through.
  foreach (panels_widgets_get_types() as $widget) {
    // Return plugin info for the widget type.
    $type = _panels_widgets_widget_content_content_type_content_type($widget);
    if ($type) {
      // Add the plugin info to the $types array, identified by
      // widget type machine-name.
      $types[$widget->type] = $type;
    }
  }
  return $types;
}

/**
 * Return a single widget type as a subtype.
 *
 * @param string $subtype_id
 *   The widget type ID
 * @param array $plugin
 *   No idea why this is added, no documentation found for it.
 *
 * @return array
 *   The plugin info array for the widget type.
 */
function panels_widgets_widget_content_content_type_content_type($subtype_id, $plugin) {
  // Load a single widget type based on the subtype id.
  $widget = panels_widgets_type_load($subtype_id);
  // Return the plugin info array based on the widget type.
  return _panels_widgets_widget_content_content_type_content_type($widget);
}

/**
 * Build an info array describing a single widget type.
 *
 * @param array $widget
 *   The single widget entity array.
 *
 * @return array
 *   The widget info array.
 */
function _panels_widgets_widget_content_content_type_content_type($widget) {
  if (empty($widget)) {
    // The $widget is empty or missing so return.
    return;
  }
  // @ToDo: Add additional fields for customisation (context etc).
  $type = array(
    'title' => $widget->label,
    'description' => !empty($widget->data['description']) ? $widget->data['description'] : '',
    'category' => !empty($widget->data['category']) ? $widget->data['category'] : 'Panels Widgets',
    'optional_view_modes' => !empty($widget->data['optional_view_modes']) ? $widget->data['optional_view_modes'] : FALSE,
  );

  return $type;
}

/**
 * The Widget Content Type Edit Form.
 *
 * @param array $form
 *   The Widget content type form array.
 * @param array &$form_state
 *   The current $form_state.
 *
 * @return array
 *   The Widget content type form array.
 */
function panels_widgets_widget_content_content_type_edit_form($form, &$form_state) {
  // Check if the widgets_type form state exists.
  if (empty($form_state['widget'])) {

    // If this is a new Pane, don't look for existing Widget.
    $widget = panels_widgets_load($form_state['conf']['pwid']);
    if (empty($widget)) {
      // Create a new Widget Object.
      $widget = panels_widgets_create_widget(array('type' => $form_state['pane']->subtype));
    }
    // Store the widget entity in the form. There should only be a single widget
    // entity per form but this will make it easier to reference later.
    $form_state['widget'][$widget->type] = $widget;
  }
  // Attach the Widget Entity to the Widget Type Entity form.
  panels_widgets_attach_form($form, $form_state);

  return $form;
}


/**
 * Implements hook_ctools_content_edit_form_submit().
 */
function panels_widgets_widget_content_content_type_edit_form_submit(&$form, &$form_state) {
  // Run through the submit handler.
  panels_widgets_form_submit_handler($form, $form_state);

  // Set the panels widget id.
  foreach ($form_state['widget'] as $type => $widget) {
    panels_widgets_save_widget($widget);
    $form_state['conf']['pwid'] = $widget->pwid;
  }
}

/**
 * Render the Widget Panel Pane.
 */
function panels_widgets_widget_content_content_type_render($subtype, $conf, $panel_args, $contexts = NULL) {
  // Load the Panels Widget Content;
  $widget = panels_widgets_load($conf['pwid']);
  // Hide the Panels Widget Label from display.
  $widget->label = '';
  // Instantiate an empty block class.
  $block = new stdClass();
  // Set the title based on the Panels Title selection.
  $block->title = $conf['override_title'] == 1 ? $conf['override_title_text'] : '';

  // Get the view mode value if it exists.
  $view_mode = field_get_items('panels_widgets', $widget, 'field_widgets_view_mode');
  $view_mode = !$view_mode ? 'full' : $view_mode[0]['value'];

  // Run the Widget through the Entity->view() method.
  $block->content = panels_widgets_view($widget, $view_mode);

  return $block;
}
