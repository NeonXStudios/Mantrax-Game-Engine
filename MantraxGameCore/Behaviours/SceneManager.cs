using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MantraxGameCore.Interfaces;

namespace MantraxGameCore.Behaviours
{
    public class SceneManager : IGame
    {
        private string word;

        public SceneManager(string RandomWord) 
        {
            word = RandomWord;

            Console.WriteLine(word);
        }

        public void Start()
        {
            Console.WriteLine("PUTO DARIL TE SALUDO DESDE C#");
        }

        public void Update()
        {
            Console.WriteLine("ESTE ES UN BUCLE RETRASADO DE MIERDA");
        }
    }
}
