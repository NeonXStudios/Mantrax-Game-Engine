using MantraxGameCore;
using MantraxGameCore.Interfaces;
using MantraxGameCore.Runtime;
using System;

namespace MantraxGameCore
{
    public class MantraxMain
    {
        public static RuntimeManager runtimeManager;

        public static void OnRunEngine()
        {
           runtimeManager = new RuntimeManager();
           
           SceneManager scene_test = new SceneManager();
           
           runtimeManager.RunStart();
        }

        public static void OnUpdateEngine()
        {
            runtimeManager.RunUpdate();
        }

        public static void OnEditionEngine()
        {
            runtimeManager.RunEdition();
        }

        public static void OnDrawUI()
        {
            runtimeManager.RunEditorUI();
        }

        public static void OnCloseEngine()
        {
            runtimeManager.RunCleaner();
        }
    }
}