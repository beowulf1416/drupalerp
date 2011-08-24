<?php
function erp_account_view($client, $account){
	
	$out = "<h2>".t("ERP Account View")."</h2>";
	$out .= "<pre>".print_r($client, true)."</pre>";
	$out .= "<pre>".print_r($account, true)."</pre>";
	
	return $out;
	
}

function erp_account_list_form($form, $form_state, $client = null){
	
	$header = array(
		"name"=>t("Name"),
		"description"=>t("Description")
	);
	$accounts = erp_account_load_multiple(false);
	foreach($accounts as $account){
		$options[$account->id] = array(
			"name"=>l($account->name,"erp/client/{$client->id}/account/{$account->id}"),
			"description"=>$account->description
		);
	}
	$options = array();
	$form["accounts"] = array(
		"#type"=>"tableselect",
		"#multiple"=>true,
		"#header"=>$header,
		"#options"=>$options
	);
	
	$actions = actions_get_all_actions();
	$options = array();
	foreach($actions as $aid=>$action){
		if($action["type"] == "erp_account"){
			$options[$aid] = t($action["label"]);
		}
	}
	$form["operation"] = array(
		"#type"=>"select",
		"#title"=>t("Operation"),
		"#required"=>true,
		"#options"=>$options
	);
	
	$form["actions"]["submit"] = array(
		"#type"=>"submit",
		"#value"=>t("Submit"),
		"#name"=>"btn-name"
	);
	
	return $form;
	
}

function erp_account_list_form_validate($form, &$form_state){

}

function erp_account_list_form_submit($form, &$form_state){

}

function erp_account_form($form, $form_state, $client = null, $account = null){

	if(is_null($client)){
		watchdog("erp_account", "Parameter 'client' must not be null", array(), WATCHDOG_ERROR);
		drupal_set_message(t("An error has occurred that will prevent you from saving this account."),"error");
	}
	
	$form["client"] = array(
		"#type"=>"value",
		"#value"=>$client
	);
	
	$types = AccountDb::account_types_get_all();
	$options = array();
	foreach($types as $type){
		$options[$type["id"]] = $type["name"];
	}
	$form["type"] = array(
		"#type"=>"select",
		"#title"=>t("Type"),
		"#required"=>true,
		"#options"=>$options,
		"#ajax"=>array(
			"callback"=>"erp_account_type_change_cb",
			"wrapper"=>"erp-acct-parent-wrapper"
		)
	);
	
	if(!is_null($account)){
		$account = new stdClass();
	}
	
	$form["account"] = array(
		"#type"=>"value",
		"#value"=>$account
	);
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#required"=>true
	);
	
	$form["desc"] = array(
		"#type"=>"textfield",
		"#title"=>t("Description"),
		"#required"=>true
	);
	
	$form["code"] = array(
		"#type"=>"textfield",
		"#title"=>t("Account code"),
		"#required"=>false
	);
	
	$accounts = array();
	$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : 0;
	if(isset($form_state["values"]["type"])){
		$accounts = erp_account_load_multiple(array(), array("type_id"=>$form_state["values"]["type"]));
	} else {
		$accounts = erp_account_load_multiple(false);
	}
	$options = array();
	foreach($accounts as $account){
		$options[$account["id"]] = $account["name"];
	}
	$form["parent"] = array(
		"#type"=>"select",
		"#title"=>t("Parent Account"),
		"#required"=>false,
		"#options"=>$options,
		"#prefix"=>"<div id=\"erp-acct-parent-wrapper\">",
		"#suffix"=>"</div><!-- #erp-acct-parent-wrapper -->"
	);
	
	field_attach_form("erp_account", $account, $form, $form_state);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-name",
		"#disabled"=>is_null($client)
	);
	
	return $form;
	
}

function erp_account_type_change_cb($form, $form_state){
	
	return $form["parent"];
	
}

function erp_account_form_validate($form, &$form_state){
	
}

function erp_account_form_submit($form, &$form_state){

	try {
		$account = $form_state["values"]["account"];
		$client = $form_state["values"]["client"];
		
		$id = isset($account->id) ? $account->id : "";
		$type = $form_state["values"]["type"];
		$name = $form_state["values"]["name"];
		$desc = $form_state["values"]["desc"];
		$code = $form_state["values"]["code"];
		
		if($id == ""){
			$account_id = AccountDb::account_add($type, $client->id, $name, $desc, $code);
			drupal_set_message(t("Successfully added new account"));
		} else {
			AccountDb::account_update($id, $client->id, $name, $desc, $code);
			drupal_set_message(t("An error has occurred while updating the account"), "error");
		}
		
	}catch(Exception $e){
		watchdog_exception("erp_account", $e);
		drupal_set_message(t("An error has occurred while processing the account"), "error");
	}	
}