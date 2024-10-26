using MantraxGameCore.Behaviours;
using MantraxGameCore.Interfaces;
using System;

namespace MantraxGameCore
{
    public class MantraxMain
    {
        public static IGame main()
        {
            return new SceneManager("hola");
        }
    }
}