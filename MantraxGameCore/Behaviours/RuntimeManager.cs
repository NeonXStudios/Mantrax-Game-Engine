using MantraxGameCore.Interfaces;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace MantraxGameCore.Runtime
{
    public class RuntimeManager
    {
        private List<MantraxBehaviour> instances = new List<MantraxBehaviour>();

        public void RunStart()
        {
            var derivedTypes = Assembly.GetExecutingAssembly()
                                       .GetTypes()
                                       .Where(t => t.IsSubclassOf(typeof(MantraxBehaviour)) && !t.IsAbstract);

            foreach (var type in derivedTypes)
            {
                var instance = (MantraxBehaviour)Activator.CreateInstance(type);
                instances.Add(instance);

                MethodInfo startMethod = type.GetMethod("Start");
                startMethod?.Invoke(instance, null);
            }
        }

        public void RunUpdate()
        {
            foreach (var instance in instances)
            {
                MethodInfo updateMethod = instance.GetType().GetMethod("Update");
                updateMethod?.Invoke(instance, null);
            }
        }

        public void RunEdition()
        {
            foreach (var instance in instances)
            {
                MethodInfo updateMethod = instance.GetType().GetMethod("OnEdition");
                updateMethod?.Invoke(instance, null);
            }
        }

        public void AddNewInstance(MantraxBehaviour _Mantrax)
        {
            instances.Add(_Mantrax);
        }
    }
}
