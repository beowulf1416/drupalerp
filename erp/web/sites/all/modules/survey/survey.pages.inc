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
	
	$form["pages"] = array(
		"#tree"=>true
	);
	
	$pages = SurveyDb::get_pages($survey->id);
	foreach($pages as $p){
		
		$page_id = $p["id"];
		
		$form["pages"]["page-{$page_id}"] = array(
			"#tree"=>true
		);
		
		if(isset($p["description"])){
			$form["pages"]["page-{$page_id}"]["desc"] = array(
				"#markup"=>t($p["description"]),
				"#prefix"=>"<div id=\"page-desc-{$page_id}\" class=\"page-desc\">",
				"#suffix"=>"</div><!-- #page-desc-{$page_id} -->"
			);
		}
		
		$questions = SurveyDb::get_questions($survey->id, $page_id);
		foreach($questions as $q){
			$question_id = $q["id"];
			$type_id = $q["type_id"];
			switch($type_id){
				case QTYPE_TEXT:{
					$form["pages"]["page-{$page_id}"]["question-{$question_id}"] = array(
						"#type"=>"textfield",
						"#title"=>t($q["text"]),
						"#description"=>isset($q["description"]) ? $q["description"] : "",
						"#required"=>isset($q["is_required"]) ? $q["is_required"] != Q_NOT_REQ_VALUE : false
					);
					break;
				}
				case QTYPE_YES_NO:{
					$form["pages"]["page-{$page_id}"]["question-{$question_id}"] = array(
						"#type"=>"radios",
						"#title"=>t($q["text"]),
						"#description"=>isset($q["description"]) ? $q["description"] : "",
						"#required"=>isset($q["is_required"]) ? $q["is_required"] != Q_NOT_REQ_VALUE : false,
						"#options"=>array(0=>t("Yes"),1=>t("No"))
					);
					break;
				}
				case QTYPE_TRUE_FALSE:{
					$form["pages"]["page-{$page_id}"]["question-{$question_id}"] = array(
						"#type"=>"radios",
						"#title"=>t($q["text"]),
						"#description"=>isset($q["description"]) ? $q["description"] : "",
						"#required"=>isset($q["is_required"]) ? $q["is_required"] != Q_NOT_REQ_VALUE : false,
						"#options"=>array(0=>t("True"),1=>t("False"))
					);
					break;
				}
				// TODO QTYPE_SELECT
				default:{
					$form["pages"]["page-{$page_id}"]["question-{$question_id}"] = array(
						"#type"=>"textfield",
						"#title"=>t($q["text"]),
						"#description"=>isset($q["description"]) ? $q["description"] : "",
						"#required"=>isset($q["is_required"]) ? $q["is_required"] != Q_NOT_REQ_VALUE : false
					);
					break;
				}
			}
		}
		
		$form["pages"]["page-{$page_id}"]["prev"] = array(
			"#type"=>"submit",
			"#name"=>"btn-previous",
			"#value"=>t("Previous")
		);
		
		$form["pages"]["page-{$page_id}"]["next"] = array(
			"#type"=>"submit",
			"#name"=>"btn-next",
			"#value"=>t("Next")
		);
		
	}
	
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
	
	if(isset($survey)){
		$form["link-survey-pages-add"] = array(
			"#markup"=>l("Add survey page","survey/{$survey->id}/page/add")
		);
	}
	
	$form["questions"] = array(
		"#tree"=>true
	);
	
	$options = array();
	$types = survey_types_get_all();
	$pages = isset($survey) ? survey_page_get_all($survey->id) : array();
	$questions = isset($survey) ? survey_questions_get_all($survey->id) : array();
	foreach($questions as $q){
		$options[$q["id"]] = array(
			"weight"=>$q["weight"],
			"page_id"=>$pages[$q["page_id"]]["title"],
			"name"=>isset($survey) ? l($q["name"],"survey/{$survey->id}/question/{$q["id"]}/edit") : $q["name"],
			"text"=>$q["text"],
			"type"=>$types[$q["type_id"]]["name"]
		);
	}
	$form["questions"] = array(
		"#type"=>"tableselect",
		"#header"=>array(
			"name"=>t("Name"),			
			"text"=>t("Text"),
			"type"=>t("Type"),
			"page_id"=>t("Page"),
			"weight"=>t("Weight"),
		),
		"#options"=>$options,
		"#empty"=>isset($survey) ? l("Add question","survey/{$survey->id}/question/add") : t("No questions")
	);
	
	if(isset($survey)){
		$form["link-question-add"] = array(
			"#markup"=>l("Add question","survey/{$survey->id}/question/add"),
			"#prefix"=>"<div id=\"link-question-add-wrapper\">",
			"#suffix"=>"</div><!-- #link-question-add-wrapper -->"
		);
	}
	
	if(isset($survey)){
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
				$survey_id = survey_add($title, $desc);
				drupal_set_message(t("Survey saved"));
				
				$form_state["redirect"] = "survey/{$survey_id}/edit";
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
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#required"=>false,
		"#default_value"=>isset($page) ? $page->description : ""
	);
	
	$form["weight"] = array(
		"#type"=>"textfield",
		"#title"=>t("Weight"),
		"#required"=>true,
		"#default_value"=>isset($page) ? $page->weight : 0
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

	$survey = $form_state["values"]["survey"];
	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			try {
				$survey_id = $survey->id;
				$title = $form_state["values"]["title"];
				$desc = $form_state["values"]["desc"];
				$weight = $form_state["values"]["weight"];
				$page_id = survey_page_add($survey_id, $title, $desc, $weight);
				drupal_set_message(t("Saved survey page"));
				
				$form_state["redirect"] = "survey/{$survey->id}/edit";
			}catch(Exception $e){
				watchdog_exception("survey", $e);
				drupal_set_message(t("An error occured while trying to save the survey page."),"error");
				
				$form_state["redirect"] = "survey/{$survey->id}/page/add";
			}
			
			break;
		}
		case "btn-cancel":{
			
			$form_state["redirect"] = "survey/{$survey->id}/edit";
			
			break;
		}
	}
	
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
		"#default_value"=>isset($question) ? $question->name : ""
	);
	
	$form["text"] = array(
		"#type"=>"textarea",
		"#title"=>t("Text"),
		"#required"=>true,
		"#default_value"=>isset($question) ? $question->text : ""
	);
	
	$form["desc"] = array(
		"#type"=>"textarea",
		"#title"=>t("Description"),
		"#required"=>false,
		"#default_value"=>isset($question) ? $question->description : ""
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
		"#default_value"=>isset($question) ? $question->type_id : QTYPE_TEXT,
		"#ajax"=>array(
			"callback"=>"survey_question_form_type_callback",
			"wrapper"=>"survey-type-ajax-result-wrapper"
		)
	);
	
	$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : QTYPE_TEXT;
	switch($type_id){
		case QTYPE_TEXT:{
			$form["example-type-text"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_YES_NO:{
			$form["example-type-yes-no"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_TRUE_FALSE:{
			$form["example-type-true-false"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_SELECT_SINGLE:{
			$form["example-type-yes-no"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		case QTYPE_SELECT_MULTI:{
			$form["example-type-yes-no"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
		default:{
			$form["example-type-text"] = array(
				"#markup"=>isset($form_state["values"]["text"]) ? "<p>".$form_state["values"]["text"]."</p>" : "<p>Why is the sky blue?</p>",
				"#prefix"=>"<div class=\"question-example\>",
				"#suffix"=>"</div><!-- .question-example -->"
			);
			break;
		}
	}
	
	$options = array();
	$pages = survey_page_get_all($survey->id);
	foreach($pages as $page){
		$options[$page["id"]] = $page["title"];
	}
	$form["page"] = array(
		"#type"=>"select",
		"#title"=>t("Page"),
		"#options"=>$options,
		"#required"=>true,
		"#default_value"=>isset($question) ? $question->page_id : 0
	);
	$form["link-page-add"] = array(
		"#markup"=>l("Add page","survey/{$survey->id}/page/add"),
		"#prefix"=>"<div class=\"link-page-add-wrapper\">",
		"#suffix"=>"</div><!-- .link-page-add-wrapper -->"
	);
	
	$form["weight"] = array(
		"#type"=>"textfield",
		"#title"=>t("Weight"),
		"#required"=>false,
		"#default_value"=>isset($question) ? $question->weight : 0
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
	
	// TODO
	
}

function survey_question_form_validate($form, &$form_state){
	
}

function survey_question_form_submit($form, &$form_state){

	switch($form_state["triggering_element"]["#name"]){
		case "btn-save":{
			
			$survey = $form_state["values"]["survey"];
			$question = isset($form_state["values"]["question"]) ? $form_state["values"]["question"] : null;
			
			try {
				$name = isset($form_state["values"]["name"]) ? $form_state["values"]["name"] : "";
				$text = isset($form_state["values"]["text"]) ? $form_state["values"]["text"] : "";
				$type_id = isset($form_state["values"]["type"]) ? $form_state["values"]["type"] : QTYPE_TEXT;
				$page_id = isset($form_state["values"]["page"]) ? $form_state["values"]["page"] : 0;
				$weight = isset($form_state["values"]["weight"]) ? $form_state["values"]["weight"] : 0;
				if(isset($question) && is_object($question) && isset($question->id)){
					
				} else {
					$question_id = survey_question_add($survey->id, $name, $text, $type_id, $page_id, $weight);
				}
				drupal_set_message(t("Saved survey question"));
				$form_state["redirect"] = "survey/{$survey->id}/edit";
			}catch(Exception $e){
				watchdog_exception("survey", $e);
				drupal_set_message(t("An error occured while trying to save the survey question."),"error");
				
				if(isset($question) && is_object($question) && isset($question->id)){
					$form_state["redirect"] = "survey/{$survey->id}/question/{$question->id}/edit";
				} else {
					$form_state["redirect"] = "survey/{$survey->id}/question/add";
				}
			}
			
			break;
		}
		case "btn-cancel":{
			$form_state["redirect"] = "survey/{$survey->id}/edit";
			break;
		}
	}
	
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