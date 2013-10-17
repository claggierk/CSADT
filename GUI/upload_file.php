<?php
	//http://www.w3schools.com/php/php_file_upload.asp
	//http://www.developphp.com/view_lesson.php?v=449
	//http://www.php.net/manual/en/function.is-uploaded-file.php
	
	function displayHelp($fileName, $allowedFileTypes)
	{ 
	   echo "Invalid file     : " . $fileName . "<br>";
	   echo "Valid file types : <br>";
	   print_r($allowedFileTypes);
	}
	
	$fileName = $_FILES["file"]["name"];
	$tempFileName = $_FILES["file"]["tmp_name"];
	$allowedFileTypes = array("txt", "xlsx", "jpg");
	$temp = explode(".", $fileName);
	$extension = end($temp);

	if(in_array($extension, $allowedFileTypes))
	{
		if($_FILES["file"]["error"] > 0)
		{
			echo "Return Code: " . $_FILES["file"]["error"] . "<br>";
		}
		else
		{
			echo "Upload: " . $fileName . "<br>";
			echo "Type: " . $_FILES["file"]["type"] . "<br>";
			echo "Size: " . ($_FILES["file"]["size"] / 1024) . " kB<br>";
			
			if(!is_uploaded_file($tempFileName))
			{
				echo "ERROR: File \"" . $fileName . "\" not uploaded<br>";
			}
			
			$destination = "uploads/" . $fileName;
			if(file_exists($destination))
			{
				echo $destination . " already exists... overwriting<br>";
			}
			
			$moveResult = move_uploaded_file($tempFileName, $destination);
			if($moveResult == false)
			{
				 echo "ERROR: File\"" . $tempFileName. "\" failed to move to \"" . $destination . "\"<br>";
			}
			if(!file_exists($destination))
			{
				echo "ERROR: " . $destination . " does not exist...";
			}
			echo "File Stored: " . $destination . "<br>";
		}
	}
	else
	{
		displayHelp($fileName, $allowedFileTypes);
	}
?>