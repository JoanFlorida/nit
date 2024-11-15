﻿#include "matrix4.h"
#include "vector4.h"

namespace nit
{
    bool operator==(const Matrix4& a, const Matrix4& b)
    {
        bool is_equal = true;

        for (u32 i = 0; i < 4; i++)
        {
            for (u32 j = 0; j < 4; j++)
            {
                if (Abs(a.m[i][j] - b.m[i][j]) <= F32_EPSILON)
                {
                    continue;
                }
                is_equal = false;
                break;
            }
        }

        return is_equal;
    }

    bool operator!=(const Matrix4& a, const Matrix4& b)
    {
        return !(a == b);
    }

    Matrix4 operator*(const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;
        for (u32 i = 0; i < 4; i++)
        {
            for (u32 j = 0; j < 4; j++)
            {
                result.m[i][j] =
                    a.m[0][j] * b.m[i][0] +
                    a.m[1][j] * b.m[i][1] +
                    a.m[2][j] * b.m[i][2] +
                    a.m[3][j] * b.m[i][3];
            }
        }
        return result;
    }

    Matrix4& operator*=(Matrix4& a, const Matrix4& b)
    {
        a = a * b;
        return a;
    }

    Vector4 operator*(const Matrix4& matrix, const Vector4& vector)
    {
        Vector4 v;
        v.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + vector.w * matrix.m[3][0];
        v.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + vector.w * matrix.m[3][1];
        v.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + vector.w * matrix.m[3][2];
        v.w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + vector.w * matrix.m[3][3];
        return v;
    }

    void SetIdentity(Matrix4& matrix)
    {
        memset(matrix.m, 0, sizeof(f32) * 16);
        matrix.m[0][0] = 1;
        matrix.m[1][1] = 1;
        matrix.m[2][2] = 1;
        matrix.m[3][3] = 1;
    }

    void SetZero(Matrix4& matrix)
    {
        memset(matrix.m, 0, sizeof(f32) * 16);
    }

    Matrix4 CreateTransform(const Vector3& position, const Vector3& rotation /*= V3_ZERO*/, const Vector3& scale /*= V3_ONE*/)
    {
        const Matrix4 identity;
        Matrix4 rotation_matrix = (rotation != V3_ZERO) ? Rotate(identity, ToRadians(rotation)) : identity;
        Matrix4 translation = Translate(rotation_matrix, position);
        Matrix4 scale_matrix = Scale(translation, scale);
        return scale_matrix;
    }

    Matrix4 Translate(const Matrix4& matrix, const Vector3& translation)
    {
        Matrix4 translate_matrix;
        translate_matrix.m[3][0] = translation.x;
        translate_matrix.m[3][1] = translation.y;
        translate_matrix.m[3][2] = translation.z;
        return translate_matrix * matrix;
    }

    Matrix4 RotateX(const Matrix4& matrix, f32 x)
    {
        Matrix4 rotate_matrix;
        rotate_matrix.n[5] =  cosf(x);
        rotate_matrix.n[9] = -sinf(x);
        rotate_matrix.n[6] =  sinf(x);
        rotate_matrix.n[10] = cosf(x);
        return rotate_matrix * matrix;
    }

    Matrix4 RotateY(const Matrix4& matrix, f32 y)
    {
        Matrix4 rotate_matrix;
        rotate_matrix.n[0] =  cosf(y);
        rotate_matrix.n[8] =  sinf(y);
        rotate_matrix.n[2] = -sinf(y);
        rotate_matrix.n[10] = cosf(y);
        return rotate_matrix * matrix;
    }

    Matrix4 RotateZ(const Matrix4& matrix, f32 z)
    {
        Matrix4 rotate_matrix;
        rotate_matrix.n[0] =  cosf(z);
        rotate_matrix.n[4] = -sinf(z);
        rotate_matrix.n[1] =  sinf(z);
        rotate_matrix.n[5] =  cosf(z);
        return rotate_matrix * matrix;
    }

    Matrix4 Rotate(const Matrix4& matrix, const Vector3& rotation)
    {
        return RotateZ(RotateY(RotateX(matrix, rotation.x), rotation.y), rotation.z);
    }

    Matrix4 Scale(const Matrix4& matrix, const Vector3& scale)
    {
        Matrix4 scale_matrix;

        scale_matrix.m[0][0] = scale.x;
        scale_matrix.m[1][1] = scale.y;
        scale_matrix.m[2][2] = scale.z;

        return matrix * scale_matrix;
    }

    f32 Determinant(const Matrix4& matrix)
    {
        return matrix.n[12] * matrix.n[9]  * matrix.n[6]  * matrix.n[3] -
               matrix.n[8]  * matrix.n[13] * matrix.n[6]  * matrix.n[3] -
               matrix.n[12] * matrix.n[5]  * matrix.n[10] * matrix.n[3] +
               matrix.n[4]  * matrix.n[13] * matrix.n[10] * matrix.n[3] +
               matrix.n[8]  * matrix.n[5]  * matrix.n[14] * matrix.n[3] -
               matrix.n[4]  * matrix.n[9]  * matrix.n[14] * matrix.n[3] -
               matrix.n[12] * matrix.n[9]  * matrix.n[2]  * matrix.n[7] +
               matrix.n[8]  * matrix.n[13] * matrix.n[2]  * matrix.n[7] +
               matrix.n[12] * matrix.n[1]  * matrix.n[10] * matrix.n[7] -
               matrix.n[0]  * matrix.n[13] * matrix.n[10] * matrix.n[7] -
               matrix.n[8]  * matrix.n[1]  * matrix.n[14] * matrix.n[7] +
               matrix.n[0]  * matrix.n[9]  * matrix.n[14] * matrix.n[7] +
               matrix.n[12] * matrix.n[5]  * matrix.n[2]  * matrix.n[11] -
               matrix.n[4]  * matrix.n[13] * matrix.n[2]  * matrix.n[11] -
               matrix.n[12] * matrix.n[1]  * matrix.n[6]  * matrix.n[11] +
               matrix.n[0]  * matrix.n[13] * matrix.n[6]  * matrix.n[11] +
               matrix.n[4]  * matrix.n[1]  * matrix.n[14] * matrix.n[11] -
               matrix.n[0]  * matrix.n[5]  * matrix.n[14] * matrix.n[11] -
               matrix.n[8]  * matrix.n[5]  * matrix.n[2]  * matrix.n[15] +
               matrix.n[4]  * matrix.n[9]  * matrix.n[2]  * matrix.n[15] +
               matrix.n[8]  * matrix.n[1]  * matrix.n[6]  * matrix.n[15] -
               matrix.n[0]  * matrix.n[9]  * matrix.n[6]  * matrix.n[15] -
               matrix.n[4]  * matrix.n[1]  * matrix.n[10] * matrix.n[15] +
               matrix.n[0]  * matrix.n[5]  * matrix.n[10] * matrix.n[15];
    }

    Matrix4 Transpose(const Matrix4& matrix)
    {
        Matrix4 transposed;
        transposed.m[0][1] = matrix.m[1][0];
        transposed.m[0][2] = matrix.m[2][0];
        transposed.m[0][3] = matrix.m[3][0];
        transposed.m[1][0] = matrix.m[0][1];
        transposed.m[1][2] = matrix.m[2][1];
        transposed.m[1][3] = matrix.m[3][1];
        transposed.m[2][0] = matrix.m[0][2];
        transposed.m[2][1] = matrix.m[1][2];
        transposed.m[2][3] = matrix.m[3][2];
        transposed.m[3][0] = matrix.m[0][3];
        transposed.m[3][1] = matrix.m[1][3];
        transposed.m[3][2] = matrix.m[2][3];
        return transposed;
    }

    Matrix4 Inverse(const Matrix4& matrix)
    {
        Matrix4 inverse;

        const f32 det = Determinant(matrix);

        if (0.0f == det) {
            NIT_LOG_WARN("Matrix has no determinant. Can not invert\n");
            return matrix;
        }

        const f32 inv_det = 1.0f / det;

        inverse.n[0] = inv_det * (matrix.n[9] * matrix.n[14] * matrix.n[7] - matrix.n[13] * matrix.n[10] * matrix.n[7] +
            matrix.n[13] * matrix.n[6] * matrix.n[11] - matrix.n[5] * matrix.n[14] * matrix.n[11] -
            matrix.n[9] * matrix.n[6] * matrix.n[15] + matrix.n[5] * matrix.n[10] * matrix.n[15]);
        inverse.n[1] = inv_det * (matrix.n[13] * matrix.n[10] * matrix.n[3] - matrix.n[9] * matrix.n[14] * matrix.n[3] -
            matrix.n[13] * matrix.n[2] * matrix.n[11] + matrix.n[1] * matrix.n[14] * matrix.n[11] +
            matrix.n[9] * matrix.n[2] * matrix.n[15] - matrix.n[1] * matrix.n[10] * matrix.n[15]);
        inverse.n[2] = inv_det * (matrix.n[5] * matrix.n[14] * matrix.n[3] - matrix.n[13] * matrix.n[6] * matrix.n[3] +
            matrix.n[13] * matrix.n[2] * matrix.n[7] - matrix.n[1] * matrix.n[14] * matrix.n[7] -
            matrix.n[5] * matrix.n[2] * matrix.n[15] + matrix.n[1] * matrix.n[6] * matrix.n[15]);
        inverse.n[3] = inv_det * (matrix.n[9] * matrix.n[6] * matrix.n[3] - matrix.n[5] * matrix.n[10] * matrix.n[3] -
            matrix.n[9] * matrix.n[2] * matrix.n[7] + matrix.n[1] * matrix.n[10] * matrix.n[7] +
            matrix.n[5] * matrix.n[2] * matrix.n[11] - matrix.n[1] * matrix.n[6] * matrix.n[11]);
        inverse.n[4] = inv_det * (matrix.n[12] * matrix.n[10] * matrix.n[7] - matrix.n[8] * matrix.n[14] * matrix.n[7] -
            matrix.n[12] * matrix.n[6] * matrix.n[11] + matrix.n[4] * matrix.n[14] * matrix.n[11] +
            matrix.n[8] * matrix.n[6] * matrix.n[15] - matrix.n[4] * matrix.n[10] * matrix.n[15]);
        inverse.n[5] = inv_det * (matrix.n[8] * matrix.n[14] * matrix.n[3] - matrix.n[12] * matrix.n[10] * matrix.n[3] +
            matrix.n[12] * matrix.n[2] * matrix.n[11] - matrix.n[0] * matrix.n[14] * matrix.n[11] -
            matrix.n[8] * matrix.n[2] * matrix.n[15] + matrix.n[0] * matrix.n[10] * matrix.n[15]);
        inverse.n[6] = inv_det * (matrix.n[12] * matrix.n[6] * matrix.n[3] - matrix.n[4] * matrix.n[14] * matrix.n[3] -
            matrix.n[12] * matrix.n[2] * matrix.n[7] + matrix.n[0] * matrix.n[14] * matrix.n[7] +
            matrix.n[4] * matrix.n[2] * matrix.n[15] - matrix.n[0] * matrix.n[6] * matrix.n[15]);
        inverse.n[7] = inv_det * (matrix.n[4] * matrix.n[10] * matrix.n[3] - matrix.n[8] * matrix.n[6] * matrix.n[3] +
            matrix.n[8] * matrix.n[2] * matrix.n[7] - matrix.n[0] * matrix.n[10] * matrix.n[7] -
            matrix.n[4] * matrix.n[2] * matrix.n[11] + matrix.n[0] * matrix.n[6] * matrix.n[11]);
        inverse.n[8] = inv_det * (matrix.n[8] * matrix.n[13] * matrix.n[7] - matrix.n[12] * matrix.n[9] * matrix.n[7] +
            matrix.n[12] * matrix.n[5] * matrix.n[11] - matrix.n[4] * matrix.n[13] * matrix.n[11] -
            matrix.n[8] * matrix.n[5] * matrix.n[15] + matrix.n[4] * matrix.n[9] * matrix.n[15]);
        inverse.n[9] = inv_det * (matrix.n[12] * matrix.n[9] * matrix.n[3] - matrix.n[8] * matrix.n[13] * matrix.n[3] -
            matrix.n[12] * matrix.n[1] * matrix.n[11] + matrix.n[0] * matrix.n[13] * matrix.n[11] +
            matrix.n[8] * matrix.n[1] * matrix.n[15] - matrix.n[0] * matrix.n[9] * matrix.n[15]);
        inverse.n[10] = inv_det * (matrix.n[4] * matrix.n[13] * matrix.n[3] - matrix.n[12] * matrix.n[5] * matrix.n[3] +
            matrix.n[12] * matrix.n[1] * matrix.n[7] - matrix.n[0] * matrix.n[13] * matrix.n[7] -
            matrix.n[4] * matrix.n[1] * matrix.n[15] + matrix.n[0] * matrix.n[5] * matrix.n[15]);
        inverse.n[11] = inv_det * (matrix.n[8] * matrix.n[5] * matrix.n[3] - matrix.n[4] * matrix.n[9] * matrix.n[3] -
            matrix.n[8] * matrix.n[1] * matrix.n[7] + matrix.n[0] * matrix.n[9] * matrix.n[7] +
            matrix.n[4] * matrix.n[1] * matrix.n[11] - matrix.n[0] * matrix.n[5] * matrix.n[11]);
        inverse.n[12] = inv_det * (matrix.n[12] * matrix.n[9] * matrix.n[6] - matrix.n[8] * matrix.n[13] * matrix.n[6] -
            matrix.n[12] * matrix.n[5] * matrix.n[10] + matrix.n[4] * matrix.n[13] * matrix.n[10] +
            matrix.n[8] * matrix.n[5] * matrix.n[14] - matrix.n[4] * matrix.n[9] * matrix.n[14]);
        inverse.n[13] = inv_det * (matrix.n[8] * matrix.n[13] * matrix.n[2] - matrix.n[12] * matrix.n[9] * matrix.n[2] +
            matrix.n[12] * matrix.n[1] * matrix.n[10] - matrix.n[0] * matrix.n[13] * matrix.n[10] -
            matrix.n[8] * matrix.n[1] * matrix.n[14] + matrix.n[0] * matrix.n[9] * matrix.n[14]);
        inverse.n[14] = inv_det * (matrix.n[12] * matrix.n[5] * matrix.n[2] - matrix.n[4] * matrix.n[13] * matrix.n[2] -
            matrix.n[12] * matrix.n[1] * matrix.n[6] + matrix.n[0] * matrix.n[13] * matrix.n[6] +
            matrix.n[4] * matrix.n[1] * matrix.n[14] - matrix.n[0] * matrix.n[5] * matrix.n[14]);
        inverse.n[15] = inv_det * (matrix.n[4] * matrix.n[9] * matrix.n[2] - matrix.n[8] * matrix.n[5] * matrix.n[2] +
                matrix.n[8] * matrix.n[1] * matrix.n[6] - matrix.n[0] * matrix.n[9] * matrix.n[6] -
                matrix.n[4] * matrix.n[1] * matrix.n[10] + matrix.n[0] * matrix.n[5] * matrix.n[10]);

        return inverse;
    }

    Matrix4 ortho_projection(f32 left, f32 right, f32 bottom, f32 top, f32 near_plane, f32 far_plane)
    {
        Matrix4 mat;
        mat.m[0][0] = 2 / (right - left);
        mat.m[1][1] = 2 / (top - bottom);
        mat.m[2][2] = -2 / (far_plane - near_plane);
        mat.m[3][0] = -(right + left) / (right - left);
        mat.m[3][1] = -(top + bottom) / (top - bottom);
        mat.m[3][2] = -(far_plane + near_plane) / (far_plane - near_plane);
        return mat;
    }

    Matrix4 ortho_projection(const f32 aspect_ratio, const f32 size, f32 near_plane, f32 far_plane)
    {
        const f32 right = aspect_ratio * size;
        const f32 left = -right;
        return ortho_projection(left, right, -size, size, near_plane, far_plane);
    }

    Matrix4 ViewProjection(const Vector3& position, const Vector3& rotation)
    {
        const Matrix4 view_matrix = CreateTransform(position, rotation);
        return Inverse(view_matrix);
    }

    Matrix4 PerspectiveProjection(f32 fov, f32 aspect, f32 near_clip, f32 far_clip)
    {
        NIT_CHECK(abs(aspect - std::numeric_limits<f32>::epsilon()) > static_cast<f32>(0));

        f32 const tan_half_fov = tan(ToRadians(fov) / static_cast<f32>(2));

        Matrix4 result;
        SetZero(result);
        result.m[0][0] = static_cast<f32>(1) / (aspect * tan_half_fov);
        result.m[1][1] = static_cast<f32>(1) / (tan_half_fov);
        result.m[2][2] = - (far_clip + near_clip) / (far_clip - near_clip);
        result.m[2][3] = - static_cast<f32>(1);
        result.m[3][2] = - (static_cast<f32>(2) * far_clip * near_clip) / (far_clip - near_clip);
        return result;
    }

    bool Decompose(const Matrix4& matrix, Vector3& position, Vector3& rotation, Vector3& scale)
    {
        Matrix4 local_matrix = matrix;
        
        if (Abs(local_matrix.m[3][3]) < F32_EPSILON)
        {
            return false;
        }

        if (Abs(local_matrix.m[0][3]) > F32_EPSILON || Abs(local_matrix.m[1][3]) > F32_EPSILON || Abs(local_matrix.m[2][3]) > F32_EPSILON)
        {
            local_matrix.m[0][3] = local_matrix.m[1][3] = local_matrix.m[2][3] = 0.0f;
            local_matrix.m[3][3] = 1.0f;
        }

        position = { local_matrix.m[3][0], local_matrix.m[3][1], local_matrix.m[3][2] };
        local_matrix.m[3][0] = local_matrix.m[3][1] = local_matrix.m[3][2] = 0.0f;

        Vector3 row[3] = {
            { local_matrix.m[0][0], local_matrix.m[0][1], local_matrix.m[0][2] },
            { local_matrix.m[1][0], local_matrix.m[1][1], local_matrix.m[1][2] },
            { local_matrix.m[2][0], local_matrix.m[2][1], local_matrix.m[2][2] }
        };

        scale.x = Lenght(row[0]);
        scale.y = Lenght(row[1]);
        scale.z = Lenght(row[2]);

        row[0] = Normalize(row[0]);
        row[1] = Normalize(row[1]);
        row[2] = Normalize(row[2]);

        rotation.y = std::asin(-row[0].z); // Pitch
        
        if (std::cos(rotation.y) > 1e-6f)
        {
            rotation.x = std::atan2(row[1].z, row[2].z); // Yaw
            rotation.z = std::atan2(row[0].y, row[0].x); // Roll
        }
        else
        {
            rotation.x = std::atan2(-row[2].x, row[1].y); // Gimbal lock
            rotation.z = 0.0f;
        }

        rotation = ToDegrees(rotation);
        return true;
    }

    Vector3 ScreenToWorldPoint(const Matrix4& projection_view_matrix, const Vector2& screen_point,
        const Vector2& window_size)
    {
        const float half_screen_width = window_size.x / 2.f;
        const float half_screen_height = window_size.y / 2.f;
        const Matrix4 inverse_mv = Inverse(projection_view_matrix);
        const float near_plane_x = (screen_point.x - half_screen_width) / half_screen_width;
        const float near_plane_y = -((screen_point.y - half_screen_height) / half_screen_height);
        const Vector4 near_plane = { near_plane_x, near_plane_y, -1, 1 };
        Vector4 near_result = inverse_mv * near_plane;
        near_result /= near_result.w;
        return { near_result.x, near_result.y, 0 };
    }
}
