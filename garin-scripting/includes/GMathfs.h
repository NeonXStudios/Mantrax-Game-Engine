#pragma once
#include <random>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

class GARINLIBS_API GMathfs
{
public:
    static float RandomFloat(float min, float max)
    {
        if (min < max)
        {
            if (max > min)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(min, max);
                double random_num = dis(gen);

                return random_num;
            }
        }

        return 0;
    }

    static float ClampFloat(float Value, float Min, float Max)
    {

        if (Value >= Max)
        {
            Value = Max;
        }

        if (Value <= Min)
        {
            Value = Min;
        }

        return Value;
    }

    static float ClampInt(int Value, int Min, int Max)
    {

        if (Value >= Max)
        {
            Value = Max;
        }

        if (Value <= Min)
        {
            Value = Min;
        }

        return Value;
    }

    static float ABS(float Value)
    {
        float abs_num = abs(Value);
        return abs_num;
    }

    // static float ACOS(float Value)
    // {
    //     float acos_num = acos(Value);
    // }

    static bool Aproximity(float a, float b, float tolerance)
    {
        return (fabs(a - b) < tolerance);
    }

    static float ASIN(float Value)
    {
        float asin_num = asin(Value);

        return asin_num;
    }

    static float Atan(float Value)
    {
        float atan_num = atan(Value);

        return atan_num;
    }

    static float Atan2(float x, float y)
    {
        float atan2_xy = atan2(y, x);

        return atan2_xy;
    }

    static float Ceil(float Value)
    {
        return ceil(Value);
    }

    static int CeilToInt(float Value)
    {
        return static_cast<int>(std::ceil(Value));
    }

    // static float Clamp01(float Value)
    // {
    //     return std::min(1.0f, std::max(0.0f, Value));
    // }

    static float ClosestPowerOfTwo(float value)
    {
        if (value <= 0)
        {
            return 0;
        }

        float logValue = log(value) / log(2.0f);

        if (pow(2.0f, logValue) == value)
        {
            return value;
        }

        float lowerPowerOfTwo = pow(2.0f, floor(logValue));
        float upperPowerOfTwo = pow(2.0f, ceil(logValue));

        if (value - lowerPowerOfTwo < upperPowerOfTwo - value)
        {
            return lowerPowerOfTwo;
        }
        else
        {
            return upperPowerOfTwo;
        }
    }

    static float SQRT(float Value)
    {
        float y = std::sqrt(Value);

        return y;
    }

    // static float SmoothDamp(float current, float target, float currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
    // {
    //     float smoothTimeClamped = std::max(0.0001f, smoothTime);
    //     float omega = 2.0f / smoothTimeClamped;
    //     float x = omega * deltaTime;
    //     float alpha = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    //     float delta = target - current;
    //     float maxDelta = maxSpeed * smoothTimeClamped;
    //     delta = std::clamp(delta, -maxDelta, maxDelta);
    //     float temp = (currentVelocity + omega * delta) * deltaTime;
    //     currentVelocity = (currentVelocity - omega * temp) * alpha;
    //     float nextValue = current + delta + temp;
    //     if ((target - current > 0.0f) == (nextValue > target))
    //     {
    //         nextValue = target;
    //         currentVelocity = 0.0f;
    //     }
    //     return nextValue;
    // }

    static float SmoothDampAngle(float current, float target, float currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
    {
        float twoPi = 2 * GMathfs::PI();
        float currentNormalized = std::fmod(current, twoPi);
        float targetNormalized = std::fmod(target, twoPi);
        float angleDiff = targetNormalized - currentNormalized;
        if (angleDiff > GMathfs::PI())
        {
            targetNormalized -= twoPi;
        }
        else if (angleDiff < -GMathfs::PI())
        {
            targetNormalized += twoPi;
        }
        float omega = 2.0f / smoothTime;
        float x = omega * deltaTime;
        float maxDelta = maxSpeed * smoothTime;
        float num = GMathfs::ClampFloat(angleDiff, -maxDelta, maxDelta);
        targetNormalized = currentNormalized + num;
        float y = (4.0f / (1.0f + omega * deltaTime * omega * deltaTime)) * deltaTime;
        float z = currentVelocity + omega * num;
        currentVelocity = (currentVelocity - omega * z) * y;
        float nextValue = targetNormalized + (currentNormalized - targetNormalized) * std::exp(-omega * deltaTime);
        return std::fmod(nextValue + twoPi, twoPi);
    }

    static float PI()
    {
        return 3.14159;
    };

    /*static double SmoothStep(double start, double end, double value) {
        value = std::clamp(value, 0.0, 1.0);
        double smoothedValue = value * value * (3 - 2 * value);
        return std::lerp(start, end, smoothedValue);
    }*/

    static double Tan(double degrees)
    {
        double radians = degrees * GMathfs::PI() / 180.0;
        return std::tan(radians);
    }
};