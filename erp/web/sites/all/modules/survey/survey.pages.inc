<?php
function survey_list(){
	
	$surveys = SurveyDb::survey_get_all();
	return theme("survey_list",array(
		"surveys"=>$surveys
	));
	
}

function survey_form($form, $form_state, $survey = null){

	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["page"] = array(
		"#tree"=>true
	);
	
	return $form;
	
}

function survey_form_validate($form, &$form_state){

}

function survey_form_submit($form, &$form_state){

}

function survey_edit_form($form, $form_state, $survey = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>$survey == null ? "" : $survey->title
	);
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#default_value"=>$survey == null ? "" : $survey->description
	);
	
	if($survey != null){
		$form["link-survey-pages-add"] = array(
			"#markup"=>l("Add survey page","survey/{$survey->id}/page/add")
		);
	}
	
	$options = array();
	$form["questions"] = array(
		"#type"=>"tableselect",
		"#header"=>array(
			"page_id"=>t("Page"),
			"text"=>t("Text"),
			"type"=>t("Type")
		),
		"#options"=>$options,
		"#empty"=>$survey == null ? t("No questions") : l("Add question","survey/{$survey->id}/question/add")
	);
	
	if($survey != null){
		$form["closed"] = array(
			"#type"=>"checkbox",
			"#title"=>t("This survey is closed"),
			"#required"=>false,
			"#default_value"=>false
		);
	}
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>t("Cancel")
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>t("Save")
	);
	
	return $form;
	
}

function survey_edit_form_validate($form, &$form_state){

}

function survey_edit_form_submit($form, &$form_state){

	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			try {
				$title = $form_state["values"]["title"];
				$desc = $form_state["values"]["desc"];
				survey_add($title, $desc);
				drupal_set_message(t("Survey saved"));
			}catch(Exception $e){
				watchdog_exception("survey", $e);
				drupal_set_message(t("An error occurred while saving the survey."));
			}
			break;
		}
	}
	
}

function survey_page_form($form, $form_state, $survey = null, $page = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["page"] = array(
		"#type"=>"value",
		"#value"=>$page
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>isset($page) ? $page->title : ""
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>t("Cancel")
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>t("Save")
	);
	
	return $form;
	
}

function survey_page_form_validate($form, &$form_state){

}

function survey_page_form_submit($form, &$form_state){

}

function survey_question_form($form, $form_state, $survey = null, $question = null){
	
	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["question"] = array(
		"#type"=>"value",
		"#value"=>$question
	);
	
	$form["name"] = array(
		"#type"=>"textfield",
		"#title"=>t("Name"),
		"#required"=>true,
		"#default_value"=>""
	);

	$types = array();
	$results = SurveyDb::get_question_types();
	foreach($results as $result){
		$types[$result["id"]] = t($result["name"]);
	}
	$form["type"] = array(
		"#type"=>"select",
		"#title"=>t("Type"),
		"#required"=>true,
		"#options"=>$types,
		"#default_value"=>0,
		"#ajax"=>array(
			"callback"=>"survey_question_form_type_callback",
			"wrapper"=>"survey-type-ajax-result-wrapper"
		)
	);
	
	$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : QTYPE_TEXT;
	$form["type-params"] = array(
		"#type"=>"fieldset",
		"#title"=>t("Parameters"),
		"#prefix"=>"<div id=\"survey-type-ajax-result-wrapper\">",
		"#suffix"=>"</div><!-- #survey-type-ajax-result-wrapper -->"
	);
	
	switch($type_id){
		case QTYPE_TEXT:{
			break;
		}
		case QTYPE_YES_NO:{
			break;
		}
		case QTYPE_TRUE_FALSE:{
			break;
		}
		case QTYPE_SELECT_SINGLE:{
			break;
		}
		case QTYPE_SELECT_MULTI:{
			break;
		}
	}
	
	$pages = array();
	$form["page"] = array(
		"#type"=>"select",
		"#title"=>t("Page"),
		"#options"=>$pages,
		"#required"=>true
	);
	$form["link-page-add"] = array(
		"#markup"=>l("Add page","survey/{$survey->id}/page/add"),
		"#prefix"=>"<div class=\"link-page-add-wrapper\">",
		"#suffix"=>"</div><!-- .link-page-add-wrapper -->"
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>"Cancel"
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>"Save"
	);
	
	return $form;
	
}

function survey_question_form_type_callback($form, $form_state){
	
	return $form["type-params"];
	
}

function survey_question_form_validate($form, &$form_state){
	
}

function survey_question_form_submit($form, &$form_state){

}

function survey_question_selection_form($form, $form_state, $survey = null, $question = null, $title = null, $value = null){

	$form["survey"] = array(
		"#type"=>"value",
		"#value"=>$survey
	);
	
	$form["question"] = array(
		"#type"=>"value",
		"#value"=>$question
	);
	
	$form["title"] = array(
		"#type"=>"textfield",
		"#title"=>t("Title"),
		"#required"=>true,
		"#default_value"=>$title
	);
	
	$form["value"] = array(
		"#type"=>"textfield",
		"#title"=>t("Value"),
		"#required"=>true,
		"#default_value"=>$value
	);
	
	$form["actions"]["cancel"] = array(
		"#type"=>"submit",
		"#name"=>"btn-cancel",
		"#value"=>t("Cancel")
	);
	
	$form["actions"]["save"] = array(
		"#type"=>"submit",
		"#name"=>"btn-save",
		"#value"=>t("Save")
	);
	
}

function survey_question_selection_form_validate($form, &$form_state){
	
}

function survey_question_selection_form_submit($form, &$form_state){

}