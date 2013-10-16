<?php
function callfire_sdk_block_click_to_subscribe_configure() {
  $form = array();

  $form['callfire_sdk_click_to_subscribe_contact_list'] = array(
    '#type' => 'select',
    '#title' => t('Contact List'),
    '#required' => TRUE,
    '#default_value' => variable_get('callfire_sdk_click_to_subscribe_contact_list'),
    '#options' => callfire_sdk_click_to_subscribe_contact_list_map(),
  );
  
  return $form;
}

function callfire_sdk_block_click_to_subscribe_save($edit = array()) {
  variable_set(
    'callfire_sdk_click_to_subscribe_contact_list',
    $edit['callfire_sdk_click_to_subscribe_contact_list']
  );
}

function callfire_sdk_block_click_to_subscribe_view() {
  $block = array();
  $block['subject'] = t('Click to Subscribe');
  
  if (callfire_sdk_has_viable_credentials() && $contact_list_id = variable_get('callfire_sdk_click_to_subscribe_contact_list')) {
    $contact_list_name = callfire_sdk_block_click_to_subscribe_contact_list_name($contact_list_id);
    if($contact_list_name) {
      $block['subject'] = t('Subscribe to') . ' ' . $contact_list_name;
    }
  }
  
  $block['content'] = drupal_get_form('callfire_sdk_block_click_to_subscribe_form');
  
  return $block;
}

function callfire_sdk_block_click_to_subscribe_form($form, &$form_state) {
  $form['subscribed_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Phone Number'),
    '#required' => TRUE,
    '#size' => 20,
    '#maxlength' => 25,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe')
  );
  
  return $form;
}

function callfire_sdk_block_click_to_subscribe_form_validate($form, &$form_state) {
  $number_length = strlen($form_state['values']['subscribed_number']);
  if($number_length <= 5 || $number_length > 25) {
    form_set_error('subscribed_number', t('Invalid phone number'));
  }
}

function callfire_sdk_block_click_to_subscribe_form_submit($form, &$form_state) {
  $number = $form_state['values']['subscribed_number'];

  $result = callfire_sdk_block_click_to_subscribe_add_to_list($number);
  if(!$result) {
    form_set_error('submit', t('Failed'));
  }
}

function callfire_sdk_block_click_to_subscribe_contact_list_map() {
  $client = callfire_sdk_rest_client('Contact');
  
  $request = $client::request('QueryContactLists');
  $response = $client->QueryContactLists($request);
  $result = $client::response($response);
  
  if (!($result instanceof CallFire\Common\Response\ResourceListInterface)) {
    return array();
  }
  
  $contact_list_map = array();
  
  foreach ($result as $contact_list) {
    $contact_list_map[$contact_list->getId()] = $contact_list->getName();
  }
  
  return $contact_list_map;
}

function callfire_sdk_block_click_to_subscribe_contact_list_name($contact_list_id) {
  // TODO: Use GetContactList

  $contact_list_map = callfire_sdk_block_click_to_subscribe_contact_list_map();
  if (isset($contact_list_map[$contact_list_id])) {
    return $contact_list_map[$contact_list_id];
  }
  
  return FALSE;
}

function callfire_sdk_block_click_to_subscribe_add_to_list($number) {
  $contact_list_id = variable_get('callfire_sdk_click_to_subscribe_contact_list');
  
  $client = callfire_sdk_rest_client('Contact');
  $request = $client::request('AddContactsToList');
  $request->setNumbers(array($number));
  
  $response = $client->AddContactsToList($contact_list_id, $request);
  $result = $client::response($response);
  if($result instanceof CallFire\Common\Response\ResourceExceptionInterface) {
    return false;
  }
  
  return true;
}
