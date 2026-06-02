// BinSerialization.h
// Binary serialization implementations removed from component headers.
// Kept for reference — not compiled or used anywhere.

#pragma once

#include "Serialization/BinSerializer.h"
#include "Components/CName.h"
#include "Components/CTransform.h"
#include "Components/CCamera.h"

namespace Twisted
{
    // Default fallback stubs (were in BinSerializer.h)
    template<typename T>
    BinSerializer BinSerialize(const T&) { return BinSerializer{}; }
    template<typename T>
    void BinDeserialize(T&, const BinSerializer&) {}

    // NameComponent (was in CName.h)
    template<typename T>
    inline void BinSerialize(const NameComponent& name, BinSerializer& buffer)
    {
        buffer.Write<std::string>(name.Name, nullptr);
    }
    template<typename T>
    inline void BinDeserialize(NameComponent& name, BinSerializer& buffer)
    {
        name.Name = buffer.Read<std::string>(nullptr);
    }

    // TransformComponent (was in CTransform.h)
    template<typename T>
    inline void BinSerialize(const TransformComponent& transform, BinSerializer& buffer)
    {
        buffer.Write<Vec3f>(transform.LocalPos, nullptr);
        buffer.Write<Vec3f>(transform.LocalScale, nullptr);
        buffer.Write<Quat>(transform.LocalRot, nullptr);

        buffer.Write<EntityID>(transform.Parent, nullptr);
        buffer.Write<std::vector<EntityID>>(transform.Children, nullptr);
    }
    template<typename T>
    inline void BinDeserialize(TransformComponent& transform, BinSerializer& buffer)
    {
        transform.LocalPos = buffer.Read<Vec3f>(nullptr);
        transform.LocalScale = buffer.Read<Vec3f>(nullptr);
        transform.LocalRot = buffer.Read<Quat>(nullptr);

        transform.Parent = buffer.Read<EntityID>(transform.GetWorld());
        transform.Children = buffer.Read<std::vector<EntityID>>(transform.GetWorld());
    }

    // CameraComponent (was in CCamera.h)
    template<typename T>
    inline void BinSerialize(const CameraComponent& camera, BinSerializer& buffer)
    {
        buffer.Write<CameraProjectionType>(camera.ProjectionType, nullptr);
        buffer.Write<float>(camera.NearPlane, nullptr);
        buffer.Write<float>(camera.FarPlane, nullptr);
        buffer.Write<float>(camera.FovDeg, nullptr);
        buffer.Write<float>(camera.AspectRatio, nullptr);
        buffer.Write<float>(camera.LeftEdge, nullptr);
        buffer.Write<float>(camera.RightEdge, nullptr);
        buffer.Write<float>(camera.BotEdge, nullptr);
        buffer.Write<float>(camera.TopEdge, nullptr);
    }
    template<typename T>
    inline void BinDeserialize(CameraComponent& camera, BinSerializer& buffer)
    {
        camera.ProjectionType = buffer.Read<CameraProjectionType>(nullptr);
        camera.NearPlane = buffer.Read<float>(nullptr);
        camera.FarPlane = buffer.Read<float>(nullptr);
        camera.FovDeg = buffer.Read<float>(nullptr);
        camera.AspectRatio = buffer.Read<float>(nullptr);
        camera.LeftEdge = buffer.Read<float>(nullptr);
        camera.RightEdge = buffer.Read<float>(nullptr);
        camera.BotEdge = buffer.Read<float>(nullptr);
        camera.TopEdge = buffer.Read<float>(nullptr);
    }

    // RendererComponent (was in BinSerializer.h as a commented-out block)
    //template<typename T>
    //inline void BinSerialize(const RendererComponent& renderer, BinSerializer& buffer)
    //{
    //    auto serMeshInfo = AssetsLayer::GetInstance()->GetObjectAssetInfo(renderer.mesh);
    //    AssetUuid meshUuid = (serMeshInfo) ? serMeshInfo->GetUuid() : AssetUuid::Invalid();
    //    buffer.Write<AssetUuid>(meshUuid, nullptr);
    //
    //    auto serMaterialInfo = AssetsLayer::GetInstance()->GetObjectAssetInfo(renderer.material);
    //    AssetUuid matUuid = (serMaterialInfo) ? serMaterialInfo->GetUuid() : AssetUuid::Invalid();
    //    buffer.Write<AssetUuid>(matUuid, nullptr);
    //}
    //template<typename T>
    //inline void BinDeserialize(RendererComponent& renderer, BinSerializer& buffer)
    //{
    //    AssetUuid serMesh = buffer.Read<AssetUuid>(nullptr);
    //    std::string meshName = "";
    //    renderer.mesh = static_cast<Mesh*>(AssetsLayer::GetInstance()->GetAssetObject(serMesh, meshName));
    //
    //    AssetUuid serMaterial = buffer.Read<AssetUuid>(nullptr);
    //    std::string materialName = "";
    //    renderer.material = static_cast<Material*>(AssetsLayer::GetInstance()->GetAssetObject(serMaterial, materialName));
    //}
}
