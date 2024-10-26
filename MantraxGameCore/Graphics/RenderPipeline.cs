using System;
using System.Runtime.CompilerServices;

public class RenderPipeline
{
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void render_all_data(float[] cameraMatrix);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void delete_from_render(IntPtr renderable); 

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void addLayer(int layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void removeLayer(int layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void test_mono();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static IntPtr add_camera();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static IntPtr add_render_texture();
}
