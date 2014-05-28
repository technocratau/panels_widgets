<?php

/**
 * @file
 * Widget Admin UI
 */

/**
 * UI Controller
 */
class PanelsWidgetsTypeUIController extends EntityDefaultUIController {

  /**
   * Override hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    // Simply overriding the default to add our own description.
    $items[$this->path]['description'] = t('Manage Widget Types and their fields.');

    return $items;
  }
}

/**
 * Generate the Widget Content Type editing form.
 */
function panels_widgets_type_form($form, &$form_state, $widget_type, $op = 'edit') {
  if ($op == 'clone') {
    $widget_type->label .= ' (cloned)';
    $widget_type->type = '';
  }
  // Widget label.
  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $widget_type->label,
    '#description' => t('The human readable name of this Widget type.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  // Widget machine name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($widget_type->type) ? $widget_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $widget_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'panels_widgets_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this widget type. It must only contain lowercase letters, numbers and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['category'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget Category'),
    '#description' => t('Provide a category for this widget to be stored inside the panels interface.'),
    '#default' => !empty($widget_type->data['category']) ? $widget_type->data['category'] : '',
  );
  $form['data']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Widget Description'),
    '#description' => t('Provide a short description for the widget.'),
    '#default_value' => !empty($widget_type->data['description']) ? $widget_type->data['description'] : '',
  );
  $form['data']['optional_view_modes'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow editors to select view mode?'),
    '#description' => t('Check this box if you wish to select the view mode when creating new content widgets in Panels.'),
    '#default_value' => !empty($widget_type->data['optional_view_modes']) ? $widget_type->data['optional_view_modes'] : '',
  );

  // Submit button.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Widget Type'),
    '#weight' => 30,
  );

  // Delete button for non-locked widget types.
  if (!$widget_type->isLocked() && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete Widget Type'),
      '#weight' => 40,
      '#limit_validation_errors' => array(),
      '#submit' => array('panels_widgets_type_form_submit_delete'),
    );
  }
  return $form;
}

/**
 * Submit callback for the admin form.
 */
function panels_widgets_type_form_submit(&$form, &$form_state) {
  // Update the widget type entity with the submitted values.
  $widget_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save the form and go back.
  $widget_type->save();
  // Redirect to the Widget Types page.
  // ToDo: Redirect to the manage fields page when creating new entity types.
  $form_state['redirect'] = 'admin/structure/widgets';
}

/**
 * Delete callback for the admin form.
 */
function panels_widgets_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/widgets/manage/' . $form['panels_widgets_type']->type . '/delete';
}
