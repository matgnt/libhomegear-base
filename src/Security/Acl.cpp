/* Copyright 2013-2017 Sathya Laufer
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

#include <error.h>
#include "Acl.h"
#include "../BaseLib.h"

namespace BaseLib
{
namespace Security
{

Acl::Acl()
{
}

Acl::~Acl()
{
}

PVariable Acl::toVariable()
{
    try
    {
        /*
         * 1. Variables
         * 2. Devices
         * 3. Rooms
         * 4. Categories
         * 5. Methods
         */

        PVariable acl = std::make_shared<Variable>(VariableType::tStruct);

        if(_variablesReadSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& device : _variablesRead)
            {
                PVariable deviceElement = std::make_shared<Variable>(VariableType::tStruct);

                for(auto& channel : device.second)
                {
                    PVariable channelElement = std::make_shared<Variable>(VariableType::tStruct);

                    for(auto& variable : channel.second)
                    {
                        channelElement->structValue->emplace(variable.first, std::make_shared<Variable>(variable.second));
                    }

                    deviceElement->structValue->emplace(std::to_string(channel.first), channelElement);
                }

                rootElement->structValue->emplace(std::to_string(device.first), deviceElement);
            }

            acl->structValue->emplace("variablesRead", rootElement);
        }

        if(_variablesWriteSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& device : _variablesWrite)
            {
                PVariable deviceElement = std::make_shared<Variable>(VariableType::tStruct);

                for(auto& channel : device.second)
                {
                    PVariable channelElement = std::make_shared<Variable>(VariableType::tStruct);

                    for(auto& variable : channel.second)
                    {
                        channelElement->structValue->emplace(variable.first, std::make_shared<Variable>(variable.second));
                    }

                    deviceElement->structValue->emplace(std::to_string(channel.first), channelElement);
                }

                rootElement->structValue->emplace(std::to_string(device.first), deviceElement);
            }

            acl->structValue->emplace("variablesWrite", rootElement);
        }

        if(_devicesReadSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& device : _devicesRead)
            {
                rootElement->structValue->emplace(std::to_string(device.first), std::make_shared<Variable>(device.second));
            }

            acl->structValue->emplace("devicesRead", rootElement);
        }

        if(_devicesWriteSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& device : _devicesWrite)
            {
                rootElement->structValue->emplace(std::to_string(device.first), std::make_shared<Variable>(device.second));
            }

            acl->structValue->emplace("devicesWrite", rootElement);
        }

        if(_roomsReadSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& room : _roomsRead)
            {
                rootElement->structValue->emplace(std::to_string(room.first), std::make_shared<Variable>(room.second));
            }

            acl->structValue->emplace("roomsRead", rootElement);
        }

        if(_roomsWriteSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& room : _roomsWrite)
            {
                rootElement->structValue->emplace(std::to_string(room.first), std::make_shared<Variable>(room.second));
            }

            acl->structValue->emplace("roomsWrite", rootElement);
        }

        if(_categoriesReadSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& category : _categoriesRead)
            {
                rootElement->structValue->emplace(std::to_string(category.first), std::make_shared<Variable>(category.second));
            }

            acl->structValue->emplace("categoriesRead", rootElement);
        }

        if(_categoriesWriteSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& category : _categoriesWrite)
            {
                rootElement->structValue->emplace(std::to_string(category.first), std::make_shared<Variable>(category.second));
            }

            acl->structValue->emplace("categoriesWrite", rootElement);
        }

        if(_methodsSet)
        {
            PVariable rootElement = std::make_shared<Variable>(VariableType::tStruct);

            for(auto& method : _methods)
            {
                rootElement->structValue->emplace(method.first, std::make_shared<Variable>(method.second));
            }

            acl->structValue->emplace("methods", rootElement);
        }

        return acl;
    }
    catch(const std::exception& ex)
    {
        throw AclException(ex.what());
    }
    catch(...)
    {
        throw AclException("Unknown error.");
    }
    return Variable::createError(-32500, "Unknown application error.");
}

void Acl::fromVariable(PVariable serializedData)
{
    try
    {
        if(!serializedData) throw AclException("serializedData is nullptr.");
        if(serializedData->type != VariableType::tStruct) throw AclException("Data is not of type Struct.");

        for(auto& rootElement : *serializedData->structValue)
        {
            if(rootElement.first == "variablesRead")
            {
                _variablesReadSet = true;
                for(auto& deviceElement : *rootElement.second->structValue)
                {
                    if(deviceElement.second->type != VariableType::tStruct) throw AclException("Device element is not of type Struct.");
                    std::string deviceString = deviceElement.first;
                    if(!Math::isNumber(deviceString, false)) throw AclException("Peer ID is not a valid number.");
                    uint64_t peerId = Math::getNumber64(deviceString, false);

                    for(auto& channelElement : *deviceElement.second->structValue)
                    {
                        if(channelElement.second->type != VariableType::tStruct) throw AclException("Channel element is not of type Struct.");
                        std::string channelString = channelElement.first;
                        if(!Math::isNumber(channelString, false)) throw AclException("Channel index is not a valid number.");
                        int32_t channel = Math::getNumber(channelString, false);

                        for(auto& variableElement: *channelElement.second->structValue)
                        {
                            if(variableElement.second->type != VariableType::tBoolean) throw AclException("Variable element is not of type bool.");
                            _variablesRead[peerId][channel][variableElement.first] = variableElement.second->booleanValue;
                        }
                    }
                }
            }
            else if(rootElement.first == "variablesWrite")
            {
                _variablesWriteSet = true;
                for(auto& deviceElement : *rootElement.second->structValue)
                {
                    if(deviceElement.second->type != VariableType::tStruct) throw AclException("Device element is not of type Struct.");
                    std::string deviceString = deviceElement.first;
                    if(!Math::isNumber(deviceString, false)) throw AclException("Peer ID is not a valid number.");
                    uint64_t peerId = Math::getNumber64(deviceString, false);

                    for(auto& channelElement : *deviceElement.second->structValue)
                    {
                        if(channelElement.second->type != VariableType::tStruct) throw AclException("Channel element is not of type Struct.");
                        std::string channelString = channelElement.first;
                        if(!Math::isNumber(channelString, false)) throw AclException("Channel index is not a valid number.");
                        int32_t channel = Math::getNumber(channelString, false);

                        for(auto& variableElement: *channelElement.second->structValue)
                        {
                            if(variableElement.second->type != VariableType::tBoolean) throw AclException("Variable element is not of type bool.");
                            _variablesWrite[peerId][channel][variableElement.first] = variableElement.second->booleanValue;
                        }
                    }
                }
            }
            else if(rootElement.first == "devicesRead")
            {
                _devicesReadSet = true;
                for(auto& deviceElement : *rootElement.second->structValue)
                {
                    if(deviceElement.second->type != VariableType::tBoolean) throw AclException("Device element is not of type bool.");
                    std::string idString = deviceElement.first;
                    if(!Math::isNumber(idString, false)) throw AclException("Device ID is not a valid number.");
                    _devicesRead[Math::getNumber64(idString, false)] = deviceElement.second->booleanValue;
                }
            }
            else if(rootElement.first == "devicesWrite")
            {
                _devicesWriteSet = true;
                for(auto& deviceElement : *rootElement.second->structValue)
                {
                    if(deviceElement.second->type != VariableType::tBoolean) throw AclException("Device element is not of type bool.");
                    std::string idString = deviceElement.first;
                    if(!Math::isNumber(idString, false)) throw AclException("Device ID is not a valid number.");
                    _devicesWrite[Math::getNumber64(idString, false)] = deviceElement.second->booleanValue;
                }
            }
            else if(rootElement.first == "roomsRead")
            {
                _roomsReadSet = true;
                for(auto& roomElement : *rootElement.second->structValue)
                {
                    if(roomElement.second->type != VariableType::tBoolean) throw AclException("Room element is not of type bool.");
                    std::string idString = roomElement.first;
                    if(!Math::isNumber(idString, false)) throw AclException("Room ID is not a valid number.");
                    _roomsRead[Math::getNumber64(idString, false)] = roomElement.second->booleanValue;
                }
            }
            else if(rootElement.first == "roomsWrite")
            {
                _roomsWriteSet = true;
                for(auto& roomElement : *rootElement.second->structValue)
                {
                    if(roomElement.second->type != VariableType::tBoolean) throw AclException("Room element is not of type bool.");
                    std::string idString = roomElement.first;
                    if(!Math::isNumber(idString, false)) throw AclException("Room ID is not a valid number.");
                    _roomsWrite[Math::getNumber64(idString, false)] = roomElement.second->booleanValue;
                }
            }
            else if(rootElement.first == "categoriesRead")
            {
                _categoriesReadSet = true;
                for(auto& categoryElement : *rootElement.second->structValue)
                {
                    if(categoryElement.second->type != VariableType::tBoolean) throw AclException("Category element is not of type bool.");
                    std::string idString = categoryElement.first;
                    if(!Math::isNumber(idString, false)) throw AclException("Category ID is not a valid number.");
                    _categoriesRead[Math::getNumber64(idString, false)] = categoryElement.second->booleanValue;
                }
            }
            else if(rootElement.first == "categoriesWrite")
            {
                _categoriesWriteSet = true;
                for(auto& categoryElement : *rootElement.second->structValue)
                {
                    if(categoryElement.second->type != VariableType::tBoolean) throw AclException("Category element is not of type bool.");
                    std::string idString = categoryElement.first;
                    if(!Math::isNumber(idString, false)) throw AclException("Category ID is not a valid number.");
                    _categoriesWrite[Math::getNumber64(idString, false)] = categoryElement.second->booleanValue;
                }
            }
            else if(rootElement.first == "methods")
            {
                _methodsSet = true;
                for(auto& methodElement : *rootElement.second->structValue)
                {
                    if(methodElement.second->type != VariableType::tBoolean) throw AclException("Method element is not of type bool.");
                    _methods[methodElement.first] = methodElement.second->booleanValue;
                }
            }
            else throw AclException("Unknown element in Struct: " + rootElement.first);
        }
    }
    catch(const std::exception& ex)
    {
        throw AclException(ex.what());
    }
    catch(...)
    {
        throw AclException("Unknown error.");
    }
}

std::string Acl::toString(int32_t indentation)
{
    if(indentation < 0) indentation = 0;
    if(indentation > 100) indentation = 100;

    std::string prefix;
    prefix.resize(indentation, ' ');

    std::ostringstream stream;
    if(_variablesReadSet)
    {
        stream << prefix << "Readable variables:" << std::endl;
        for(auto& device : _variablesRead)
        {
            stream << prefix << "  * Device " << device.first << ":" << std::endl;
            for(auto& channel : device.second)
            {
                stream << prefix << "    - Channel " << channel.first << ":" << std::endl;

                for(auto& variable : channel.second)
                {
                    stream << prefix << "      " << variable.first << ": " << variable.second << std::endl;
                }
            }
        }
        stream << std::endl;
    }

    if(_variablesWriteSet)
    {
        stream << prefix << "Writeable variables:" << std::endl;
        for(auto& device : _variablesWrite)
        {
            stream << prefix << "  * Device " << device.first << ":" << std::endl;
            for(auto& channel : device.second)
            {
                stream << prefix << "    - Channel " << channel.first << ":" << std::endl;

                for(auto& variable : channel.second)
                {
                    stream << prefix << "      " << variable.first << ": " << (variable.second ? "accept" : "deny") << std::endl;
                }
            }
        }
        stream << std::endl;
    }

    if(_devicesReadSet)
    {
        stream << prefix << "Readable devices:" << std::endl;
        for(auto& device : _devicesRead)
        {
            stream << prefix << "  * " << device.first << ": " << (device.second ? "accept" : "deny") << std::endl;
        }
    }

    if(_devicesWriteSet)
    {
        stream << prefix << "Writeable devices:" << std::endl;
        for(auto& device : _devicesWrite)
        {
            stream << prefix << "  * " << device.first << ": " << (device.second ? "accept" : "deny") << std::endl;
        }
    }

    if(_roomsReadSet)
    {
        stream << prefix << "Readable room IDs:" << std::endl;
        for(auto& room : _roomsRead)
        {
            stream << prefix << "  * " << room.first << ": " << (room.second ? "accept" : "deny") << std::endl;
        }
    }

    if(_roomsWriteSet)
    {
        stream << prefix << "Writeable room IDs:" << std::endl;
        for(auto& room : _roomsWrite)
        {
            stream << prefix << "  * " << room.first << ": " << (room.second ? "accept" : "deny") << std::endl;
        }
    }

    if(_categoriesReadSet)
    {
        stream << prefix << "Readable category IDs:" << std::endl;
        for(auto& category : _categoriesRead)
        {
            stream << prefix << "  * " << category.first << ": " << (category.second ? "accept" : "deny") << std::endl;
        }
    }

    if(_categoriesWriteSet)
    {
        stream << prefix << "Writeable category IDs:" << std::endl;
        for(auto& category : _categoriesWrite)
        {
            stream << prefix << "  * " << category.first << ": " << (category.second ? "accept" : "deny") << std::endl;
        }
    }

    if(_methodsSet)
    {
        stream << prefix << "Executable RPC methods:" << std::endl;
        for(auto& method : _methods)
        {
            std::string methodString = method.first == "*" ? "all" : method.first;
            stream << prefix << "  * " << methodString << ": " << (method.second ? "accept" : "deny") << std::endl;
        }
    }

    return stream.str();
}

AclResult Acl::checkCategoriesReadAccess(std::set<uint64_t>& categories)
{
    try
    {
        if(!_categoriesReadSet) return AclResult::notInList;

        AclResult categoryResult = AclResult::notInList;
        for(auto& categoryId : categories)
        {
            auto categoriesIterator = _categoriesRead.find(categoryId);
            if(categoriesIterator != _categoriesRead.end())
            {
                categoryResult = categoriesIterator->second ? AclResult::accept : AclResult::deny;
                if(categoryResult == AclResult::deny) return categoryResult; //Deny access
            }
        }

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkCategoriesWriteAccess(std::set<uint64_t>& categories)
{
    try
    {
        if(!_categoriesWriteSet) return AclResult::notInList;

        AclResult categoryResult = AclResult::notInList;
        for(auto& categoryId : categories)
        {
            auto categoriesIterator = _categoriesWrite.find(categoryId);
            if(categoriesIterator != _categoriesWrite.end())
            {
                categoryResult = categoriesIterator->second ? AclResult::accept : AclResult::deny;
                if(categoryResult == AclResult::deny) return categoryResult; //Deny access
            }
        }

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkCategoryReadAccess(uint64_t categoryId)
{
    try
    {
        if(!_categoriesReadSet) return AclResult::notInList;

        auto categoriesIterator = _categoriesRead.find(categoryId);
        if(categoriesIterator != _categoriesRead.end()) return categoriesIterator->second ? AclResult::accept : AclResult::deny;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkCategoryWriteAccess(uint64_t categoryId)
{
    try
    {
        if(!_categoriesWriteSet) return AclResult::notInList;

        auto categoriesIterator = _categoriesWrite.find(categoryId);
        if(categoriesIterator != _categoriesWrite.end()) return categoriesIterator->second ? AclResult::accept : AclResult::deny;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkDeviceReadAccess(std::shared_ptr<Systems::Peer> peer)
{
    try
    {
        if(!peer) return AclResult::error;
        if(!_roomsReadSet && !_categoriesReadSet && !_devicesReadSet) return AclResult::notInList;

        AclResult roomResult = AclResult::notInList;
        if(_roomsReadSet)
        {
            auto roomId = peer->getRoom();
            if(roomId != 0)
            {
                auto roomsIterator = _roomsRead.find(roomId);
                if(roomsIterator != _roomsRead.end())
                {
                    roomResult = roomsIterator->second ? AclResult::accept : AclResult::deny;
                    if(roomResult == AclResult::deny) return roomResult; //Deny access
                }
            }
        }
        else roomResult = AclResult::accept;

        AclResult categoryResult = AclResult::notInList;
        if(_categoriesReadSet)
        {
            for(auto& categoryId : peer->getCategories())
            {
                auto categoriesIterator = _categoriesRead.find(categoryId);
                if(categoriesIterator != _categoriesRead.end())
                {
                    categoryResult = categoriesIterator->second ? AclResult::accept : AclResult::deny;
                    if(categoryResult == AclResult::deny) return categoryResult; //Deny access
                }
            }
        }
        else categoryResult = AclResult::accept;

        AclResult deviceResult = AclResult::notInList;
        if(_devicesReadSet)
        {
            auto devicesIterator = _devicesRead.find(peer->getID()); //Check specific access first in case of "no access".
            if(devicesIterator != _devicesRead.end())
            {
                deviceResult = devicesIterator->second ? AclResult::accept : AclResult::deny;
                if(deviceResult == AclResult::deny) return deviceResult; //Deny access
            }

            if(deviceResult == AclResult::notInList)
            {
                devicesIterator = _devicesRead.find(0);
                if(devicesIterator != _devicesRead.end())
                {
                    deviceResult = devicesIterator->second ? AclResult::accept : AclResult::deny;
                    if(deviceResult == AclResult::deny) return deviceResult; //Deny access
                }
            }
        }
        else deviceResult = AclResult::accept;

        if(roomResult == AclResult::accept && categoryResult == AclResult::accept && deviceResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkDeviceWriteAccess(std::shared_ptr<Systems::Peer> peer)
{
    try
    {
        if(!peer) return AclResult::error;
        if(!_roomsWriteSet && !_categoriesWriteSet && !_devicesWriteSet) return AclResult::notInList;

        AclResult roomResult = AclResult::notInList;
        if(_roomsWriteSet)
        {
            auto roomId = peer->getRoom();
            if(roomId != 0)
            {
                auto roomsIterator = _roomsWrite.find(roomId);
                if(roomsIterator != _roomsWrite.end())
                {
                    roomResult = roomsIterator->second ? AclResult::accept : AclResult::deny;
                    if(roomResult == AclResult::deny) return roomResult; //Deny access
                }
            }
        }
        else roomResult = AclResult::accept;

        AclResult categoryResult = AclResult::notInList;
        if(_categoriesWriteSet)
        {
            for(auto& categoryId : peer->getCategories())
            {
                auto categoriesIterator = _categoriesWrite.find(categoryId);
                if(categoriesIterator != _categoriesWrite.end())
                {
                    categoryResult = categoriesIterator->second ? AclResult::accept : AclResult::deny;
                    if(categoryResult == AclResult::deny) return categoryResult; //Deny access
                }
            }
        }
        else categoryResult = AclResult::accept;

        AclResult deviceResult = AclResult::notInList;
        if(_devicesWriteSet)
        {
            auto devicesIterator = _devicesWrite.find(peer->getID()); //Check specific access first in case of "no access".
            if(devicesIterator != _devicesWrite.end())
            {
                deviceResult = devicesIterator->second ? AclResult::accept : AclResult::deny;
                if(deviceResult == AclResult::deny) return deviceResult; //Deny access
            }

            if(deviceResult == AclResult::notInList)
            {
                devicesIterator = _devicesRead.find(0);
                if(devicesIterator != _devicesRead.end())
                {
                    deviceResult = devicesIterator->second ? AclResult::accept : AclResult::deny;
                    if(deviceResult == AclResult::deny) return deviceResult; //Deny access
                }
            }
        }
        else deviceResult = AclResult::accept;

        if(roomResult == AclResult::accept && categoryResult == AclResult::accept && deviceResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkMethodAccess(std::string& methodName)
{
    try
    {
        if(!_methodsSet) return AclResult::notInList;

        auto methodsIterator = _methods.find(methodName); //Check specific access first in case of "no access".
        if(methodsIterator != _methods.end()) return methodsIterator->second ? AclResult::accept : AclResult::deny;

        methodsIterator = _methods.find("*");
        if(methodsIterator != _methods.end()) return methodsIterator->second ? AclResult::accept : AclResult::deny;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkMethodAndCategoryReadAccess(std::string& methodName, uint64_t categoryId)
{
    try
    {
        if(!_methodsSet && !_categoriesReadSet) return AclResult::notInList;

        AclResult categoryResult = AclResult::notInList;
        if(_categoriesReadSet)
        {
            auto categoriesIterator = _categoriesRead.find(categoryId); //Check specific access first in case of "no access".
            if(categoriesIterator != _categoriesRead.end())
            {
                categoryResult = categoriesIterator->second ? AclResult::accept : AclResult::deny;
                if(!categoriesIterator->second) return categoryResult; //Deny access
            }
        }
        else categoryResult = AclResult::accept;

        auto methodResult = checkMethodAccess(methodName);
        if(methodResult == AclResult::deny || methodResult == AclResult::error) return methodResult; //Deny access

        if(categoryResult == AclResult::accept && methodResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkMethodAndCategoryWriteAccess(std::string& methodName, uint64_t categoryId)
{
    try
    {
        if(!_methodsSet && !_categoriesWriteSet) return AclResult::notInList;

        AclResult categoryResult = AclResult::notInList;
        if(_categoriesWriteSet)
        {
            auto categoriesIterator = _categoriesWrite.find(categoryId); //Check specific access first in case of "no access".
            if(categoriesIterator != _categoriesWrite.end())
            {
                categoryResult = categoriesIterator->second ? AclResult::accept : AclResult::deny;
                if(!categoriesIterator->second) return categoryResult; //Deny access
            }
        }
        else categoryResult = AclResult::accept;

        auto methodResult = checkMethodAccess(methodName);
        if(methodResult == AclResult::deny || methodResult == AclResult::error) return methodResult; //Deny access

        if(categoryResult == AclResult::accept && methodResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkMethodAndRoomReadAccess(std::string& methodName, uint64_t roomId)
{
    try
    {
        if(!_methodsSet && !_roomsReadSet) return AclResult::notInList;

        AclResult roomResult = AclResult::notInList;
        if(_roomsReadSet)
        {
            auto roomIterator = _roomsRead.find(roomId); //Check specific access first in case of "no access".
            if(roomIterator != _roomsRead.end())
            {
                roomResult = roomIterator->second ? AclResult::accept : AclResult::deny;
                if(!roomIterator->second) return roomResult; //Deny access
            }
        }
        else roomResult = AclResult::accept;

        auto methodResult = checkMethodAccess(methodName);
        if(methodResult == AclResult::deny || methodResult == AclResult::error) return methodResult; //Deny access

        if(roomResult == AclResult::accept && methodResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkMethodAndRoomWriteAccess(std::string& methodName, uint64_t roomId)
{
    try
    {
        if(!_methodsSet && !_roomsWriteSet) return AclResult::notInList;

        AclResult roomResult = AclResult::notInList;
        if(_roomsWriteSet)
        {
            auto roomIterator = _roomsWrite.find(roomId); //Check specific access first in case of "no access".
            if(roomIterator != _roomsWrite.end())
            {
                roomResult = roomIterator->second ? AclResult::accept : AclResult::deny;
                if(!roomIterator->second) return roomResult; //Deny access
            }
        }
        else roomResult = AclResult::accept;

        auto methodResult = checkMethodAccess(methodName);
        if(methodResult == AclResult::deny || methodResult == AclResult::error) return methodResult; //Deny access

        if(roomResult == AclResult::accept && methodResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkMethodAndDeviceWriteAccess(std::string& methodName, uint64_t peerId)
{
    try
    {
        if(!_methodsSet && !_devicesWriteSet) return AclResult::notInList;

        AclResult deviceResult = AclResult::notInList;
        if(_devicesWriteSet)
        {
            auto deviceIterator = _devicesWrite.find(peerId); //Check specific access first in case of "no access".
            if(deviceIterator != _devicesWrite.end())
            {
                deviceResult = deviceIterator->second ? AclResult::accept : AclResult::deny;
                if(!deviceIterator->second) return deviceResult; //Deny access
            }
        }
        else deviceResult = AclResult::accept;

        auto methodResult = checkMethodAccess(methodName);
        if(methodResult == AclResult::deny || methodResult == AclResult::error) return methodResult; //Deny access

        if(deviceResult == AclResult::accept && methodResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkRoomReadAccess(uint64_t roomId)
{
    try
    {
        if(!_roomsReadSet) return AclResult::notInList;

        auto roomsIterator = _roomsRead.find(roomId); //Check specific access first in case of "no access".
        if(roomsIterator != _roomsRead.end()) return roomsIterator->second ? AclResult::accept : AclResult::deny;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkRoomWriteAccess(uint64_t roomId)
{
    try
    {
        if(!_roomsWriteSet) return AclResult::notInList;

        auto roomsIterator = _roomsWrite.find(roomId); //Check specific access first in case of "no access".
        if(roomsIterator != _roomsWrite.end()) return roomsIterator->second ? AclResult::accept : AclResult::deny;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

AclResult Acl::checkVariableReadAccess(std::shared_ptr<Systems::Peer> peer, int32_t channel, std::string& variableName)
{
    try
    {
        if(!_variablesReadSet && !_devicesReadSet && !_roomsReadSet && !_categoriesReadSet) return AclResult::notInList;

        auto deviceResult = checkDeviceReadAccess(peer);
        if(deviceResult == AclResult::deny || deviceResult == AclResult::error) return deviceResult; //Deny access

        AclResult variableResult = AclResult::notInList;
        if(_variablesReadSet)
        {
            /*auto devicesIterator = _devicesWrite.find(peer->getID()); //Check specific access first in case of "no access".
            if(devicesIterator != _devicesWrite.end())
            {
                deviceResult = devicesIterator->second ? AclResult::accept : AclResult::deny;
                if(deviceResult == AclResult::deny) return deviceResult; //Deny access
            }

            if(deviceResult == AclResult::notInList)
            {
                devicesIterator = _devicesRead.find(0);
                if(devicesIterator != _devicesRead.end())
                {
                    deviceResult = devicesIterator->second ? AclResult::accept : AclResult::deny;
                    if(deviceResult == AclResult::deny) return deviceResult; //Deny access
                }
            }*/
        }
        else variableResult = AclResult::accept;

        //if(roomResult == AclResult::accept && categoryResult == AclResult::accept && deviceResult == AclResult::accept) return AclResult::accept;

        return AclResult::notInList;
    }
    catch(const std::exception& ex)
    {
    }
    catch(...)
    {
    }
    return AclResult::error;
}

}
}
