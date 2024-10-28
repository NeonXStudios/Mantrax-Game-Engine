using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MantraxGameCore;

namespace MantraxGameCore
{
    public class SceneManager : MantraxBehaviour
    {
        public void Start()
        {
            Console.WriteLine("My First Script C# Mantrax");
        }

        public void Update()
        {
            Console.WriteLine($"Delta Time: {Time.Delta}");
        }

        public void OnEdition()
        {
            Console.WriteLine("Im on edition mode");
        }
    }
}
