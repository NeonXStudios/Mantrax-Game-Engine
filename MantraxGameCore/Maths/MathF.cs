using MantraxGameCore;
using System;
using System.Runtime.InteropServices;

public static class MathF
{
    public static float Clamp(float value, float min, float max)
    {
        return Math.Max(min, Math.Min(max, value));
    }

    public static double Power (float BaseValue, float Exponent)
    {
        return (double)Math.Pow(BaseValue, Exponent);
    }

    public static float PI
    {
        get
        {
            return (float)Math.PI;
        }    
    } 

    public static float EpsilonFloat()
    {
        return float.Epsilon;
    }

    public static double EpsilonDouble()
    {
        return double.Epsilon;
    }

    public static double Rag2Deg(double Radians)
    {
        return Radians * (180.0f / MathF.PI);
    }

    public static float NegativeInfinity()
    {
        return float.NegativeInfinity;
    }

    public static float Infinity()
    {
        return float.PositiveInfinity;
    }

    public static double Abs (float Value)
    {
        return Math.Abs(Value);
    }

    public static double Acos (float Value)
    {
        return Math.Abs(Value);
    }

    public static bool Approximately(float A, float B, float Tolerance = 1e-6f)
    {
        return MathF.Abs(A - B) < Tolerance;
    }

    public static double Asin (double Value)
    {
        return Math.Asin(Value);
    }

    public static double Atan(double Value)
    {
        return Math.Atan(Value);
    }

    public static double Atan2(double A, double B)
    {
        return Math.Atan2(A, B);
    }

    public static double Ceiling(double Value)
    {
        return Math.Ceiling(Value);
    }

    public static int CeilingToInt(float Value)
    {
        return (int)MathF.Ceiling(Value);
    }

    public static float Clamp01(float Value)
    {
        if (Value < 0)
        {
            return 0;
        }

        if (Value > 1f)
        {
            return 1f;
        }

        return Value;
    }

    public static int ClosestPowerOfTwo (int Value)
    {
        if (Value < 1)
        {
            return 1;
        }

        int power = 1;

        while (power < Value) 
        { 
            power *= 2;
        }

        if (power == Value)
        {
            return power;
        }

        int previousPower = power / 2;

        return (Value - previousPower < power - Value) ? previousPower : power;
    }

    public static double Cos(double angleInRadians)
    {
        return Math.Cos(angleInRadians);
    }

    public static float DeltaAngle(float current, float target)
    {
        float delta = (target - current) % 360f;

        if (delta > 180f)
        {
            delta -= 360f;
        }
        else if (delta < -180f)
        {
            delta += 360f;
        }

        return delta;
    }

    public static double Exp(double exponent)
    {
        return Math.Exp(exponent);
    }

    public static double Floor(double value)
    {
        return Math.Floor(value);
    }

    public static int FloorToInt(float value)
    {
        return (int)Math.Floor(value);
    }

    public static float GammaToLinearSpace(float value)
    {
        if (value <= 0.04045f)
            return value / 12.92f;
        else
            return (float)Math.Pow((value + 0.055f) / 1.055f, 2.4f);
    }

    public static float HalfToFloat(ushort half)
    {
        int sign = (half >> 15) & 0x1;
        int exponent = (half >> 10) & 0x1F;
        int mantissa = half & 0x3FF;

        if (exponent == 0)
        {
            if (mantissa == 0) return sign == 0 ? 0 : -0;
            else
            {
                while ((mantissa & 0x400) == 0)
                {
                    mantissa <<= 1;
                    exponent--;
                }
                exponent++;
                mantissa &= ~0x400;
            }
        }
        else if (exponent == 31)
        {
            if (mantissa == 0) return sign == 0 ? float.PositiveInfinity : float.NegativeInfinity;
            else return float.NaN;
        }

        exponent += 112;
        mantissa <<= 13;
        int floatBits = (sign << 31) | (exponent << 23) | mantissa;

        byte[] bytes = BitConverter.GetBytes(floatBits);
        return BitConverter.ToSingle(bytes, 0);
    }


    public static float InverseLerp(float a, float b, float value)
    {
        if (a == b) return 0f;
        return MathF.Clamp((value - a) / (b - a), 0f, 1f);
    }

    public static bool IsPowerOfTwo(int value)
    {
        return (value > 0) && ((value & (value - 1)) == 0);
    }

    public static float Lerp(float a, float b, float t)
    {
        return a + (b - a) * MathF.Clamp(t, 0f, 1f);
    }

    public static float LerpUnclamped(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    public static float LinearToGammaSpace(float value)
    {
        return value <= 0.0031308f ? value * 12.92f : (float)(1.055 * Math.Pow(value, 1 / 2.4) - 0.055);
    }

    public static double Log(double value, double baseValue)
    {
        return Math.Log(value, baseValue);
    }

    public static double Log10(double value)
    {
        return Math.Log10(value);
    }

    public static float Max(float a, float b)
    {
        return Math.Max(a, b);
    }

    public static int Max(int a, int b)
    {
        return Math.Max(a, b);
    }

    public static float Min(float a, float b)
    {
        return Math.Min(a, b);
    }

    public static int Min(int a, int b)
    {
        return Math.Min(a, b);
    }

    public static float MoveTowards(float current, float target, float maxDelta)
    {
        if (Math.Abs(target - current) <= maxDelta) return target;
        return current + Math.Sign(target - current) * maxDelta;
    }

    public static float MoveTowardsAngle(float current, float target, float maxDelta)
    {
        float delta = DeltaAngle(current, target);
        if (Math.Abs(delta) <= maxDelta) return target;
        return current + Math.Sign(delta) * maxDelta;
    }

    public static int NextPowerOfTwo(int value)
    {
        if (value < 1) return 1;
        int power = 1;
        while (power < value)
        {
            power <<= 1;
        }
        return power;
    }


    public static float PingPong(float t, float length)
    {
        return length - Math.Abs((t % (length * 2)) - length);
    }

    public static float Pow(float f, float p)
    {
        return (float)Math.Pow(f, p);
    }

    public static float Round(float f)
    {
        return (float)Math.Round(f);
    }

    public static int RoundToInt(float f)
    {
        return (int)Math.Round(f);
    }

    public static int Sign(float f)
    {
        return (f < 0) ? -1 : (f > 0) ? 1 : 0;
    }

    public static float Sin(float angle)
    {
        return (float)Math.Sin(angle);
    }

    public static float SmoothDamp(float current, float target, ref float currentVelocity, float smoothTime, float maxSpeed = float.PositiveInfinity)
    {
        float deltaTime = Time.Delta;
        smoothTime = Math.Max(0.0001f, smoothTime);
        float omega = 2f / smoothTime;
        float x = omega * deltaTime;
        float exp = 1f / (1f + x + 0.48f * x * x + 0.235f * x * x * x);
        float change = current - target;
        target = current - change;
        float temp = (currentVelocity + omega * change) * deltaTime;
        currentVelocity = (currentVelocity - omega * temp) * exp;
        return target + (change + temp) * exp;
    }

    public static float SmoothDampAngle(float current, float target, ref float currentVelocity, float smoothTime, float maxSpeed = float.PositiveInfinity)
    {
        target = current + DeltaAngle(current, target);
        return SmoothDamp(current, target, ref currentVelocity, smoothTime, maxSpeed);
    }

    public static float SmoothStep(float from, float to, float t)
    {
        t = MathF.Clamp(t, 0f, 1f);
        return from + (to - from) * t * t * (3f - 2f * t);
    }

    public static float Sqrt(float f)
    {
        return (float)Math.Sqrt(f);
    }

    public static float Tan(float angle)
    {
        return (float)Math.Tan(angle);
    }

}