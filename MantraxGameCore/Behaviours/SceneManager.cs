using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using MantraxGameCore;

public class SceneManager : MantraxBehaviour
{
    public string _Value = "New Object";

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
        GUIEditor.Begin("A");
        _Value = GUIEditor.InputField("hola", _Value);

        if (GUIEditor.Button("TEST CLIC"))
        {
            Console.WriteLine("CLIC ME BUTTON");
            Entity g = EngineCore.NewObject();
            g.Name = _Value;

            Console.WriteLine($"Entity: {g.Name}");
        }

        GUIEditor.End();
    }
}