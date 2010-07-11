// Copyright (C) 2009 Christian Stehno
// No rights reserved: this software is in the public domain.

#include "testUtils.h"

using namespace irr;
using namespace core;

bool simple_xml( irr::io::IFileSystem * fs )
{
	io::IXMLReaderUTF8* reader = fs->createXMLReaderUTF8("media/test.xml");
	if (!reader)
	{
		logTestString("Could not create XML reader.\n");
		return false;
	}

	const core::stringc expected[] = {
		"a", "b", "c"
	};

	bool retVal = true;
	u32 i=0;
	while(reader->read())
	{
		if (reader->getNodeType() == io::EXN_ELEMENT)
		{
			if (expected[i++] != reader->getNodeName())
			{
				logTestString("Did not find expected string in XML element name.\n");
				retVal = false;
				break;
			}
		}
	}

	reader->drop();

	return retVal;
}

// CDATA should return everything between "![CDATA[" and "]]>" as it's in the file
bool cdata( irr::io::IFileSystem * fs )
{
	io::IXMLReaderUTF8* reader = fs->createXMLReaderUTF8("media/cdata.xml");
	if (!reader)
	{
		logTestString("Could not create XML reader.\n");
		return false;
	}

	const core::stringc textNode("text");
	core::array< core::stringc > compareStrings;
	compareStrings.push_back("simple");
	compareStrings.push_back("");
	compareStrings.push_back("] ]> ");
	compareStrings.push_back("]\n]> ");
	compareStrings.push_back("\nNewlines\n\tand tabs\n\t\tgogogo");
	compareStrings.push_back("&&#@#$%*()@#$%*()#$%*(");
	compareStrings.push_back("& &  && &&& &a &ü &ä &ö &&#");

	bool result = true;
	size_t count = 0;
	while(reader->read())
	{
		if (reader->getNodeType() == io::EXN_ELEMENT)
		{
			if ( core::stringc(reader->getNodeName()) == textNode )
			{
				while(reader->read())
				{
					if (reader->getNodeType() == io::EXN_CDATA)
					{
						core::stringc data = reader->getNodeData();
						core::stringc name = reader->getNodeName();
						if ( count == compareStrings.size() )
						{
							logTestString("too many cdata elements for reading in %s:%d\n", __FILE__, __LINE__);
						}
						else if ( count < compareStrings.size() )
						{
							core::stringc cmpString(compareStrings[count]);
							
							// some (unused) variables to ease debugging
							// const c8* dataRaw = data.c_str();
							// const c8* cmpRaw = cmpString.c_str();
							if ( cmpString != data )
							{
								result = false;
								logTestString("cdata read failed for string %d in %s:%d\n", count, __FILE__, __LINE__);
							}
						}
						++count;
					}
					if ( reader->getNodeType() == io::EXN_ELEMENT_END )
					{
						break;
					}
				}
			}
		}
	}

	reader->drop();

	return result;
}

/** Tests for XML handling */
bool testXML(void)
{
	IrrlichtDevice *device = createDevice(video::EDT_NULL, dimension2du(400, 200));

	bool result = true;

	result &= simple_xml(device->getFileSystem());
	result &= cdata(device->getFileSystem());

	device->drop();
	return result;
}
