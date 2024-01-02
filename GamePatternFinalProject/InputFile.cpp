#include "InputFile.h"

InputFile::InputFile(string fName): fileName(fName),file(fName)
{
	if (!file.is_open())//check if the file can not be opened, the following message is diplayed
		std::cerr << "Error opening file: " << fileName<<std::endl;
	//cerr does the same thing as cout but its imediatly displayed in the console
}

list<Location*> InputFile::locqwe()
{
	return allLocations;
}

void InputFile::readFile()
{
	
	
	// Use a while loop together with the getline() function to read the file line by line
	while (std::getline(file, line)) {
		// Output the text from the file
		if (line.empty()) {
			newBlock = true;
			//keyword = " ";
			
			
		}
		

		if (line.find("Item:") != std::string::npos) {
			keyword = "Item";
			name=line.substr(6);
		}
		else if (line.find("Location:") != std::string::npos) {
			keyword = "Location";
			numberOfLocation=stoi(line.substr(10));
		}


		if(newBlock == false) {
			
			if (keyword == "Item") {
				setInput();//the input is for description and contents 
				
				if (line.find("Keys:") != std::string::npos) {

					temp = line.substr(6);
					keyItem=findItem(temp);
					
				}
				
				
			}
			else if (keyword == "Location") {
				if (line.find("Name:") != std::string::npos) {

					name = line.substr(6);
				

				}else
				{
				setInput();//the input is for description and contents 
				string direction;
				int charNum = 0;
				
				charNum = line.find_first_of(" ");
				direction = line.substr(0, charNum);
				if (searchDirection(direction))
				{
					
					if (line.find_first_of(",") != -1) {
						keyItemsMap.insert({ direction, findItem(line.substr(charNum + 4)) });

					}
					else
						keyItemsMap.insert({ direction,nullptr });

					connectionsMap.insert({ direction,stoi(line.substr(charNum + 1,1)) });
				}
				}

			}
			
		}
		else
		{
			if (name != " ") {

				if (keyword == "Item")
					processInputItem();

				else if (keyword == "Location")
					processInputLocation();
			}

			clearVariables();
		}

		


	}

	// Close the file
	file.close();
}


Item* InputFile::findItem(string temp)
{
	for (auto i : allItems)
	{
		if (i->getName() == temp) {
			return i;
		}
	}
	return nullptr;
}

void InputFile::setInput()
{
	if (line.find("Description:") != std::string::npos) {

		description = line.substr(13);
	}
	else if (line.find("Contents:") != std::string::npos) {

		temp = line.substr(10);//it was 9 before
		string tempItem;
		int i = 0;
		while (temp[i] != '\0') {

			if (temp[i] == ',') {
				contents.push_back(findItem(tempItem));
				tempItem = "";
				i += 2;
			}
			tempItem += temp[i];

			i++;
		}
		
		contents.push_back(findItem(tempItem));

	}
}

list<Item*> InputFile::qwer()
{
	return allItems;
}

bool InputFile::searchDirection(string direction)
{
	for(auto i: listOfDirections)
		if (i == direction) {
			return true;
		}
	return false;
}

void InputFile::processInputItem()
{
	Item* item;
	if (contents.empty()) {

		 item = new Item(name, description);
	}
	else {
		 item = new Container(name, description, keyItem, contents);
	}

	std::cout << std::endl;
	/*std::cout<<item->getName();
	std::cout<<item->getDescription();*/
	std::cout << std:: endl;
	//if (item != nullptr)
	allItems.push_back(item);
}

void InputFile::processInputLocation()
{
	Location* location = new Location(numberOfLocation, name, description, 
		contents,keyItemsMap,connectionsMap);
	if (numberOfLocation == 1)
		Player::changeLocation(location);
	allLocations.push_back(location);
}

void InputFile::clearVariables()
{
	newBlock = false;
	name = " ";
	description = " ";
	contents.clear();
	keyItem = nullptr;
	keyItemsMap.clear();
	connectionsMap.clear();
	
}
