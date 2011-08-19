<?php
function erp_client_form($form, $form_state, $client=null){

	if(is_null($client)){
		$client = new stdClass();
	}
	
	$form["client"] = array(
		"#type"=>"value",
		"#value"=>$client
	);
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#description"=>t("Client name"),
		"#required"=>true
	);
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#required"=>false
	);
	
	field_attach_form("erp_client", $client, $form, $form_state);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#value"=>t("Save"),
		"#name"=>"btn-save"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#value"=>t("Cancel"),
		"#name"=>"btn-cancel"
	);
	
	return $form;
	
}

function erp_client_form_validate($form, &$form_state){
	
	if($form_state["triggering_element"]["#name"] != "btn-cancel"){
		entity_form_field_validate("erp_client", $form, $form_state);
	}
	
}

function erp_client_form_submit($form, &$form_state){
	
	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{

			$client = $form_state["values"]["client"];
			if(isset($client->id)){
				
			} else {
				$name = check_plain($form_state["values"]["name"]);
				$desc = check_plain($form_state["values"]["desc"]);
				try {
					$id = ClientDB::add_client($name, $desc);
					
					$client->id = $id;
					$client->name = $name;
					$client->description = $desc;
					
					entity_form_submit_build_entity("erp_client", $client, $form, $form_state);
					
					drupal_set_message(t("Client information saved"));
					
					$form_state["redirect"] = "erp/client/{$id}/view";
					
				}catch(Exception $e){
					watchdog_exception("erp_client", $e);
					drupal_set_message(t("Unable to save client information"),"error");
				}
			}
			
			break;
		}
		case "btn-cancel":{
			break;
		}
	}
	
}

function erp_client_view($client){
	
	return theme("erp_client_item",array("client"=>$client));
	
}

function erp_client_list_form($form, $form_state){
	
	$header = array(
		"name"=>t("Name"),
		"enabled"=>t("Enabled")
	);
	$clients = erp_client_load_multiple(false);
	$options = array();
	foreach($clients as $client){
		$options[$client->id] = array(
			"name"=>l($client->name,"erp/client/{$client->id}"),
			"enabled"=>$client->enabled == 0 ? "True" : "False"
		);
	}
	
	$form["clients"] = array(
		"#type"=>"tableselect",
		"#header"=>$header,
		"#options"=>$options,
		"#empty"=>t("No clients available"),
		"#multiple"=>true
	);
	
	$actions = actions_get_all_actions();
	$options = array();
	foreach($actions as $aid=>$action){
		if($action["type"] == "erp_client"){
			$options[$aid] = t($action["label"]);
		}
	}
	$form["action"] = array(
		"#type"=>"select",
		"#title"=>t("Action"),
		"#options"=>$options,
		"#required"=>true
	);
	
	$form["actions"]["submit"] = array(
		"#type"=>"submit",
		"#value"=>t("Submit"),
		"#name"=>"btn-submit"
	);
	
	return $form;
	
}

function erp_client_list_form_validate($form, &$form_state){

}

function erp_client_list_form_submit($form, &$form_state){

	foreach($form_state["values"]["clients"] as $client_id){
		$client = erp_client_load($client_id);
		actions_do($form_state["values"]["action"], $client, null, null, null);
	}
	
}