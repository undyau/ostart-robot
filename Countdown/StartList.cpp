#include "stdafx.h"
#include "StartList.h"
#include "tinyxml2.h"


CStartList::CStartList()
{
}


CStartList::~CStartList()
{
}

bool CStartList::Load(CString a_FileName)
{
	m_StartTimes.empty();
	tinyxml2::XMLDocument doc;
	doc.LoadFile(a_FileName);

	// Structure of the XML file:
	// - Element "StartList"      the root Element, which is the 
	//                       FirstChildElement of the Document
	// - - Element "TITLE"   child of the root PLAY Element
	// - - - Text            child of the TITLE Element

	// Navigate to the title, using the convenience function,
	// with a dangerous lack of error checking.
	tinyxml2::XMLElement* root = doc.FirstChildElement("StartList");
	if (root == nullptr)
		{
		AfxMessageBox("Couldn't process file : " + a_FileName + ". It doesn't contain XML element StartList.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}

	// Iterate over all classes
	tinyxml2::XMLElement* oclass = root->FirstChildElement("ClassStart");
	if (oclass == nullptr)
		{
		AfxMessageBox("Couldn't process file : " + a_FileName + ". It doesn't contain XML element StartList.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
		return false;
		}
	while (oclass)
		{

		}
	AfxMessageBox("Couldn't process file : " + a_FileName + ". It doesn't contain XML element ClassStart.", MB_OK | MB_APPLMODAL | MB_ICONEXCLAMATION);
	printf("Name of play (1): %s\n", title);

	// Text is just another Node to TinyXML-2. The more
	// general way to get to the XMLText:
	XMLText* textNode = doc.FirstChildElement("PLAY")->FirstChildElement("TITLE")->FirstChild()->ToText();
	title = textNode->Value();
	printf("Name of play (2): %s\n", title);
	return false;
}


/*
<PersonStart>
<Person sex="M">
<Id>5075</Id>
<Name>
<Family>Harding</Family>
<Given>Stephen</Given>
</Name>
<BirthDate>1999-01-01</BirthDate>
</Person>
<Organisation>
<Id>247</Id>
<Name>NZ L</Name>
<ShortName>NZ L</ShortName>
<Country code="NZL">NZL</Country>
</Organisation>
<Start raceNumber="1">
<BibNumber>128</BibNumber>
<StartTime>2017-09-24T10:26:00.000</StartTime>
<ControlCard>2033334</ControlCard>
<AssignedFee>
<Fee>
<Name>Actual</Name>
<Amount>27.00</Amount>
</Fee>
</AssignedFee>
</Start>
</PersonStart>
*/