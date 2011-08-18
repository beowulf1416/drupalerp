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
				$name = $form_state["values"]["name"];
				$desc = $form_state["values"]["desc"];
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
	
	return "<pre>".print_r($client,true)."</pre>";
	
// 	return theme("erp_client",$client);
	
}