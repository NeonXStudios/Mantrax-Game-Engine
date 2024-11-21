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
    public float A = 0;
    public float B = 50;
    public Vector3 pos_a = new Vector3(0, 0, 0);
    public Vector3 pos_b = new Vector3(50, 50, 50);
    Entity new_obj;

    public void Start()
    {
        Console.WriteLine("My First Script C# Mantrax");
    }

    public void Update()
    {
    }

    public void OnEdition()
    {
        //A = MathF.Lerp(A, B, 0.1f * Time.Delta);

        //Console.WriteLine($"VALUE: {A}");

        //pos_a = Vector3.Lerp(pos_a, pos_b, 0.1f * Time.Delta);
        //float distance = Vector3.Distance(pos_a, pos_b);

        //Console.WriteLine($"X: {pos_a.x}");
        //Console.WriteLine($"Y: {pos_a.y}");
        //Console.WriteLine($"Z: {pos_a.z}");

        //Console.WriteLine($"DISTANCE: {distance}");
    }

    public void OnClean()
    {
        Console.WriteLine("BYE FROM C#");
    }

    public void OnDrawUIEditor()
    {
        //GUIEditor.Begin("A");
        //_Value = GUIEditor.InputField("hola", _Value);

        //if (GUIEditor.Button("TEST CLIC"))
        //{
        //    Console.WriteLine("CLIC ME BUTTON");

        //    Entity g = EngineCore.NewObject();
        //    g.Name = _Value;
        //    g.Tag = "Hello";
        //    new_obj = g;

        //    Console.WriteLine($"Entity: {g.Name}");
        //}

        //GUIEditor.End();
    }
}