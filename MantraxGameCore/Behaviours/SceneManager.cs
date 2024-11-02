using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using MantraxGameCore;

public class SceneManager : MantraxBehaviour
{
    public void Start()
    {
        Console.WriteLine("My First Script C# Mantrax");
    }

    public void Update()
    {
    }

    public void OnEdition()
    {
    }

    public void OnClean()
    {
        Console.WriteLine("BYE FROM C#");
    }

    public void OnDrawUIEditor()
    {
        GUIEditor.Begin();
        GUIEditor.End();
    }
}