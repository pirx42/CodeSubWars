// Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
// Released under the MIT license. (see license.txt)


#pragma once


namespace CodeSubWars
{

  class PythonContext
  {
    public:
      typedef std::shared_ptr<PythonContext> PtrType;


      struct MakeCurrentLock
      {
          MakeCurrentLock(PyThreadState* state);
          ~MakeCurrentLock();

        protected:
          PyThreadState* m_State;
      };


      static std::shared_ptr<PythonContext> create();
      static std::shared_ptr<PythonContext> createEmpty();

      ~PythonContext();

      static void destroyAllRemaining();

      MakeCurrentLock makeCurrent();

      //void call(std::function<void(void)> f);

      void destroy();

    protected:
      PythonContext();

      void initialize();

      PyThreadState* m_State;

      static std::vector<std::weak_ptr<PythonContext>> m_UsedPythonContexts;
  };


}