<?php
function erp_contact_form($form, $form_state, $client=null, $contact=null){
	
	if(is_null($contact)){
		$contact = new stdClass();
	}
	
	$form["client"] = array(
		"#type"=>"value",
		"#value"=>$client
	);
	
	$form["contact"] = array(
		"#type"=>"value",
		"#value"=>$contact
	);
	
	$form["name-first"] = array(
		"#type"=>"textfield",
		"#title"=>t("First name"),
		"#required"=>true,
		"#maxlength"=>100
	);
	
	$form["name-last"] = array(
		"#type"=>"textfield",
		"#title"=>t("Last name"),
		"#required"=>true,
		"#maxlength"=>100
	);
	
	$form["name-middle"] = array(
		"#type"=>"textfield",
		"#title"=>t("Middle"),
		"#required"=>false,
		"#maxlength"=>100
	);
	
	$form["name-prefix"] = array(
		"#type"=>"textfield",
		"#title"=>t("Prefix"),
		"#required"=>false,
		"#maxlength"=>50
	);
	
	$form["name-suffix"] = array(
		"#type"=>"textfield",
		"#title"=>t("Sufffix"),
		"#required"=>false,
		"#maxlength"=>50
	);
	
	$form["uid"] = array(
		"#type"=>"textfield",
		"#title"=>t("User name"),
		"#required"=>false
	);
	
	field_attach_form("erp_contact", $contact, $form, $form_state);
	
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

function erp_contact_form_validate($form, &$form_state){
	
	if($form_state["triggering_element"]["#name"] != "btn-cancel"){
		entity_form_field_validate("erp_contact", $form, $form_state);
	}

}

function erp_contact_form_submit($form, &$form_state){

	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			try {
				
				$last_name = check_plain($form_state["values"]["name-last"]);
				$first_name = check_plain($form_state["values"]["name-first"]);
				$middle_name = check_plain($form_state["values"]["name-middle"]);
				$prefix = check_plain($form_state["values"]["name-prefix"]);
				$suffix = check_plain($form_state["values"]["name-suffix"]);
				$uid = $form_state["values"]["uid"];
				
				$client = $form_state["values"]["client"];
				$contact_id = ContactDb::contact_add(
					$last_name, 
					$first_name, 
					$middle_name, 
					$prefix, 
					$suffix,
					$client->id,
					$uid);
				
				$contact = new stdClass();
				$contact->id = $contact_id;
				$contact->last_name = $last_name;
				$contact->first_name = $first_name;
				$contact->middle_name = $middle_name;
				$contact->prefix = $prefix;
				$contact->suffix = $suffix;
				$contact->uid = $uid;
				
				entity_form_submit_build_entity("erp_contact", $contact, $form, $form_state);
				
				drupal_set_message(t("Successfully added erp contact"));
				
				$form_state["redirect"] = "erp/client/{$client->id}/contact/{$contact_id}/view";
			} catch(Exception $e){
				watchdog_exception("erp_contact", $e);
				
				drupal_set_message(t("Unable to add erp contact"),"error");
			}
			break;
		}
	}
	
}

function erp_contact_delete_form($form, $form_state, $client=null, $contact=null){
	
	$form["msg"] = array(
		"#markup"=>"<div class=\"form-msg-warning\">".
			t("Are you sure you want to delete %contact?",
				array("%contact"=>"//TODO")).
				"</div><!-- .form-msg-warning -->"
	);
	
	$form["actions"]["yes"] = array(
		"#type"=>"submit",
		"#value"=>t("Yes"),
		"#name"=>"btn-yes"
	);
	
	$form["actions"]["no"] = array(
		"#type"=>"submit",
		"#value"=>t("No"),
		"#name"=>"btn-no"
	);
	
	return $form;
	
}

function erp_contact_delete_form_validate($form, &$form_state){

}

function erp_contact_delete_form_submit($form, &$form_state){

}

function erp_contact_list_form($form, $form_state, $client=null){
	
	$form["client"] = array(
		"#type"=>"value",
		"#value"=>$client
	);
	
	$header = array(
		"view_link"=>t("View"),
		"last_name"=>t("Last name"),
		"first_name"=>t("First name"),
		"middle_name"=>t("Middle name")
	);
	$contacts = erp_contact_load_multiple(false);
	$options = array();
	foreach($contacts as $contact){
		$options[$contact->id] = array(
			"view_link"=>l("View","erp/client/{$contact->client_id}/contact/{$client->id}"),
			"last_name"=>$contact->last_name,
			"first_name"=>$contact->first_name,
			"middle_name"=>$contact->middle_name
		);
	}
	$form["contacts"] = array(
		"#type"=>"tableselect",
		"#header"=>$header,
		"#options"=>$options,
		"#multiple"=>true
	);
	
	$actions = actions_get_all_actions();
	$options = array();
	foreach($actions as $aid=>$action){
		if($action["type"] == "erp_contact"){
			$options[$aid] = t($action["label"]);
		}
	}
	$form["actions"] = array(
		"#type"=>"select",
		"#title"=>t("Action"),
		"#options"=>$options,
		"#required"=>true
	);
	
	$form["actions"]["submit"] = array(
		"#type"=>"submit",
		"#value"=>t("Submit"),
		"#name"=>"btn-name"
	);
	
}

function erp_contact_list_form_validate($form, &$form_state){
	
}

function erp_contact_list_form_submit($form, &$form_state){

}