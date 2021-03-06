/* Copyright 2013-2019 Homegear GmbH
 *
 * libhomegear-base is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * libhomegear-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with libhomegear-base.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU Lesser General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
*/

#include "ParameterGroup.h"
#include "../BaseLib.h"

namespace BaseLib
{
namespace DeviceDescription
{

ParameterGroup::ParameterGroup(BaseLib::SharedObjects* baseLib)
{
	_bl = baseLib;
}

ParameterGroup::~ParameterGroup()
{
	parameters.clear();
	parametersOrdered.clear();
	scenarios.clear();
	lists.clear();
}

void ParameterGroup::parseXml(xml_node<>* node)
{
    parseAttributes(node);
    parseElements(node);
}

void ParameterGroup::parseAttributes(xml_node<>* node)
{
	for(xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
	{
		std::string attributeName(attr->name());
		std::string attributeValue(attr->value());
		if(attributeName == "id")
		{
			id = attributeValue;
		}
		else if(attributeName == "memoryAddressStart") memoryAddressStart = Math::getNumber(attributeValue);
		else if(attributeName == "memoryAddressStep") memoryAddressStep = Math::getNumber(attributeValue);
		else if(attributeName != "memoryAddressStart" && attributeName != "memoryAddressStep" && attributeName != "peerChannelMemoryOffset" && attributeName != "channelMemoryOffset" && attributeName != "peerAddressMemoryOffset" && attributeName != "maxLinkCount") _bl->out.printWarning("Warning: Unknown attribute for \"parameterGroup\": " + attributeName);
	}
}

void ParameterGroup::parseElements(xml_node<>* node)
{
	for(xml_node<>* subNode = node->first_node(); subNode; subNode = subNode->next_sibling())
	{
		std::string nodeName(subNode->name());
		if(nodeName == "parameter")
		{
			PParameter parameter = std::make_shared<Parameter>(_bl, shared_from_this());
			parameter->parseXml(subNode);
			if(!parameter->id.empty())
			{
				parameters.insert(std::pair<std::string, PParameter>(parameter->id, parameter));
				parametersOrdered.push_back(parameter);
			}
			if(parameter->physical->list > -1) lists[parameter->physical->list].push_back(parameter);
			if(parameter->parameterGroupSelector) parameterGroupSelector = parameter;
		}
		else if(nodeName == "scenario")
		{
			PScenario scenario(new Scenario(_bl, subNode));
			if(!scenario->id.empty()) scenarios.insert(std::pair<std::string, PScenario>(scenario->id, scenario));
		}
		else if(nodeName != "linkScenario") _bl->out.printWarning("Warning: Unknown node in \"parameterGroup\": " + nodeName);
	}
}

ParameterGroup::Type::Enum ParameterGroup::typeFromString(std::string type)
{
	HelperFunctions::toLower(HelperFunctions::trim(type));
	if(type == "master" || type == "config") return Type::Enum::config;
	else if(type == "values" || type == "variables") return Type::Enum::variables;
	else if(type == "link") return Type::Enum::link;
	return Type::Enum::none;
}

PParameter ParameterGroup::getParameter(std::string id)
{
	Parameters::iterator parameterIterator = parameters.find(id);
	if(parameterIterator == parameters.end()) return PParameter();
	return parameterIterator->second;
}

void ParameterGroup::getIndices(uint32_t startIndex, uint32_t endIndex, int32_t list, std::vector<PParameter>& result)
{
	try
	{
		result.clear();
		if(list < 0) return;
		Lists::iterator listIterator = lists.find(list);
		if(listIterator == lists.end()) return;
		for(std::vector<PParameter>::iterator i = listIterator->second.begin(); i != listIterator->second.end(); ++i)
		{
			if((*i)->physical->endIndex >= startIndex && (*i)->physical->startIndex <= endIndex) result.push_back(*i);
		}
	}
	catch(const std::exception& ex)
    {
    	_bl->out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
    	_bl->out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
}

ConfigParameters::ConfigParameters(BaseLib::SharedObjects* baseLib) : ParameterGroup(baseLib)
{
}

Variables::Variables(BaseLib::SharedObjects* baseLib) : ParameterGroup(baseLib)
{
}

LinkParameters::LinkParameters(BaseLib::SharedObjects* baseLib) : ParameterGroup(baseLib)
{
}

void LinkParameters::parseXml(xml_node<>* node)
{
    for(xml_attribute<>* attr = node->first_attribute(); attr; attr = attr->next_attribute())
    {
        std::string attributeName(attr->name());
        std::string attributeValue(attr->value());
        if(attributeName == "id")
        {
            id = attributeValue;
        }
        else if(attributeName == "memoryAddressStart") memoryAddressStart = Math::getNumber(attributeValue);
        else if(attributeName == "memoryAddressStep") memoryAddressStep = Math::getNumber(attributeValue);
        else if(attributeName == "peerChannelMemoryOffset") peerChannelMemoryOffset = Math::getNumber(attributeValue);
        else if(attributeName == "channelMemoryOffset") channelMemoryOffset = Math::getNumber(attributeValue);
        else if(attributeName == "peerAddressMemoryOffset") peerAddressMemoryOffset = Math::getNumber(attributeValue);
        else if(attributeName == "maxLinkCount") maxLinkCount = Math::getNumber(attributeValue);
        else _bl->out.printWarning("Warning: Unknown attribute for \"linkParameters\": " + attributeName);
    }
    parseElements(node);
}

}
}
