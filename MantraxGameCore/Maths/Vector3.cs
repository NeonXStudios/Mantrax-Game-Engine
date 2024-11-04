using System;
using System.Runtime.CompilerServices;

public class Vector3 : IDisposable
{
    private IntPtr nativePtr;

    public Vector3(float x = 0, float y = 0, float z = 0)
    {
        nativePtr = CreateVector3(x, y, z);
    }

    public float x
    {
        get => GetVector3X(nativePtr);
        set => SetVector3X(nativePtr, value);
    }

    public float y
    {
        get => GetVector3Y(nativePtr);
        set => SetVector3Y(nativePtr, value);
    }

    public float z
    {
        get => GetVector3Z(nativePtr);
        set => SetVector3Z(nativePtr, value);
    }

    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern IntPtr CreateVector3(float x, float y, float z);

    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern float GetVector3X(IntPtr vec);
    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern float GetVector3Y(IntPtr vec);
    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern float GetVector3Z(IntPtr vec);

    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern void SetVector3X(IntPtr vec, float x);
    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern void SetVector3Y(IntPtr vec, float y);
    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern void SetVector3Z(IntPtr vec, float z);

    [MethodImpl(MethodImplOptions.InternalCall)]
    private static extern void DeleteVector3(IntPtr vec);

    public void Dispose()
    {
        if (nativePtr != IntPtr.Zero)
        {
            DeleteVector3(nativePtr);
            nativePtr = IntPtr.Zero;
        }
    }

    ~Vector3()
    {
        Dispose();
    }

    public static Vector3 Lerp(Vector3 A, Vector3 B, float T)
    {
        A.x = MathF.Lerp(A.x, B.x, T);
        A.y = MathF.Lerp(A.y, B.y, T);
        A.z = MathF.Lerp(A.z, B.z, T);

        return A;
    }

    public static Vector3 Slerp(Vector3 A, Vector3 B, float T)
    {
        float dot = A.x * B.x + A.y * B.y + A.z * B.z;

        dot = MathF.Clamp(dot, -1.0f, 1.0f);

        float theta = (float)MathF.Acos(dot) * T;

        Vector3 relativeVec = new Vector3(
            B.x - A.x * dot,
            B.y - A.y * dot,
            B.z - A.z * dot
        );

        relativeVec = Normalize(relativeVec);

        return new Vector3(
            A.x * (float)MathF.Cos(theta) + relativeVec.x * (float)MathF.Sin(theta),
            A.y * (float)MathF.Cos(theta) + relativeVec.y * (float)MathF.Sin(theta),
            A.z * (float)MathF.Cos(theta) + relativeVec.z * (float)MathF.Sin(theta)
        );
    }

    public static Vector3 Normalize(Vector3 v)
    {
        float length = MathF.Sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        return new Vector3(v.x / length, v.y / length, v.z / length);
    }

    public static float Distance(Vector3 A, Vector3 B)
    {
        float deltaX = B.x - A.x;
        float deltaY = B.y - A.y;
        float deltaZ = B.z - A.z;

        return MathF.Sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
    }
}
