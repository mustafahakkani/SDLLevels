#include "Resource.h"

Resource::Resource()
{
    val1 = 0;
    val2 = 0.1;
    val3 = { };
    subResource = nullptr;
    asset = nullptr;
}

Resource::~Resource()
{
}

void Resource::AssignNonDefaultValues()
{
}

void Resource::ToString()
{
    cout << "BASE RESOURCE" << endl;
}
void Resource::SerializeAsset(std::ostream& _stream, Asset* _asset)
{
    byte guidLength = 0;
    if (_asset != nullptr)
    {
        guidLength = (byte)_asset->GetGUID().length();
    }

    _stream.write(reinterpret_cast<char*>(&guidLength), sizeof(guidLength));
    if (guidLength > 0)
    {
        _stream.write(_asset->GetGUID().c_str(), guidLength);
    }
}

void Resource::DeserializeAsset(std::istream& _stream, Asset*& _asset)
{
    byte guidLength = 0;
    _stream.read(reinterpret_cast<char*>(&guidLength), sizeof(byte));
    if (guidLength > 0)
    {
        char guid[255];
        _stream.read(guid, guidLength);
        guid[guidLength] = 0; // null terminate string
        _asset = AssetController::Instance().GetAsset(string(guid));
    }
}

void Resource::Serialize(std::ostream& _stream)
{
}

void Resource::Deserialize(std::istream& _stream)
{
}
