<?php
class SurveyDb {
	
	public static function post_install(){
		
		$values = array(
				array("id"=>QTYPE_TEXT,"name"=>"text"),
				array("id"=>QTYPE_YES_NO,"name"=>"yes/no"),
				array("id"=>QTYPE_TRUE_FALSE,"name"=>"true/false"),
				array("id"=>QTYPE_SELECT_SINGLE,"name"=>"single-select"),
				array("id"=>QTYPE_SELECT_MULTI,"name"=>"multi-select")
			);
		
		$qry = db_insert("survey_question_types")
			->fields(array(
				"id",
				"name"
			));
		foreach($values as $v){
			$qry->values($v);
		}
		$results = $qry->execute();
		
	}
	
	public static function survey_get_all(){
		
		$qry = db_select("surveys","a")
			->fields("a")
			->isNull("a.closed")
 			->execute();
		
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function survey_add($title, $desc){
		
		db_insert("surveys")
			->fields(array(
				"title"=>$title,
				"description"=>$desc,
				"created"=>REQUEST_TIME
			))
			->execute();
		
	}
	
	public static function get_question_types(){
		
		$qry = db_select("survey_question_types","a")
			->fields("a")
			->execute();
		$results = $qry->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function get_questions($survey_id){
		
		$results = db_select("survey_questions","a")
			->fields("a")
			->condition("survey_id", $survey_id, "=")
			->execute()
			->fetchAll(PDO::FETCH_ASSOC);
		return $results;
		
	}
	
	public static function get_pages($survey_id){
		
		
		
	}
	
}