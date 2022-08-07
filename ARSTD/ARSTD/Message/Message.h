// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once

#include <memory>

namespace ARSTD
{

  template <typename ObjectType>
  class Message
  {
    public:
      Message() {};
      virtual ~Message() {};

      static void broadcastMessage(typename std::shared_ptr<ObjectType> pObject, Message& message, int nMaxTreeDepth = 9999);

      static void sendMessage(typename std::shared_ptr<ObjectType> pObject, Message& message);

      virtual void evaluateObject(typename std::shared_ptr<ObjectType> pObject) {}

    protected:
      Message(const Message& other);
  };



  template <typename ObjectType>
  void Message<ObjectType>::broadcastMessage(typename std::shared_ptr<ObjectType> pObject, Message<ObjectType>& message, int nMaxTreeDepth)
  {
    if (nMaxTreeDepth < 0 || !pObject)
      return;

    message.evaluateObject(pObject);

    if (nMaxTreeDepth <= 0)
      return;

    ObjectType::ChildIterator it;
    ObjectType::ChildRange range = pObject->getChildRange();
    for (it = range.first; it != range.second; ++it)
    {
      std::shared_ptr<ObjectType> pChild = std::dynamic_pointer_cast<ObjectType>(*it);
      if (pChild)
      {
        broadcastMessage(pChild, message, nMaxTreeDepth - 1);
      }
    }
  }


  template <typename ObjectType>
  void Message<ObjectType>::sendMessage(typename std::shared_ptr<ObjectType> pObject, Message<ObjectType>& message)
  {
    if (!pObject)
      return;

    message.evaluateObject(pObject);
  }

} // namespace ARSTD
