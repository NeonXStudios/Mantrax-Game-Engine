using MantraxGameCore.Runtime;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MantraxGameCore
{
    public class MantraxBehaviour
    {
        public MantraxBehaviour()
        {
             MantraxMain.runtimeManager.AddNewInstance(this);
        }

        public void InvokeFun(string methodName, object[] parameters, int seconds)
        {
            Task.Run(async () =>
            {
                await Task.Delay(seconds * 1000); 

                MethodInfo method = GetType().GetMethod(methodName);
                if (method != null)
                {
                    method.Invoke(this, parameters);
                }
                else
                {
                    Console.WriteLine($"Method {methodName} not found on {GetType().Name}");
                }
            });
        }
    }
}
