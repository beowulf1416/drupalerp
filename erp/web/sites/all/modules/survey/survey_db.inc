<?php
class SurveyDb {
	
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