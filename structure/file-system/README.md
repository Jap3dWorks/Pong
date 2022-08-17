---
description: namespace Pong::serializer
---

# File Data

file\_data is the serializer system, also includes serializable structs that can represent geometries and nodes.

_Needs real testing for write and for read data._

## serial\_data.h

_`#include "Pong/filedata/serial_data"`_

Here is the the most high level functions, only this header should be included.

### Create serializer for custom objects

Override function  Pong::serializer::serialize using a Custom class as param&

```cpp
struct Custom {
   uint32_t attr_1;
   uint32_t attr_2;
}

template<typename Archive>
Pong::serializer::serialize(Archive& ar, Custom &custom, Pong::serializer::Version version) {
   ar & custom.attr_1;
   ar & custom.attr_2;
}
```

### Create Struct data

You can use **SERIALIZABLE**, **FIELD** and **IMPL\_SERIALIZE** macros to create a default serialize implementation for structs.

```cpp
struct CurveData {    
    SERIALIZABLE (            
        FIELD(RegId, uid),            
        FIELD(Curve, curve)    
        )
};
IMPL_SERIALIZE(CurveData);
```

## AssetDescriptors

**IAssetDescription** and **OAssetDescription** classes will define an structure to be serialized in a document.

Descriptors represents a set of elements like meshes, transforms and materials.

Descriptors should be registered using **REG\_DESCRIPTOR(class, version)** macro.

```cpp
class OAssetDescriptor;
REG_DESCRIPTOR(OAssetDescriptor, 1);
```

When a Descriptor is registered can be serialized using **OSerializer::operator<<.**&#x20;

To serialize a descriptor into a file.

<pre class="language-cpp"><code class="lang-cpp"><strong>Pong::serializer::OSSerializer(os) &#x3C;&#x3C; Pong::serializer::OAssetDescriptor();</strong></code></pre>

Also you can use save\_file to serialize a descriptor into a file.

```cpp
Pong::serializer::save_file(OAssetDescriptor descriptor, const char* file_name);
```

When a descriptor is saved its **version** is appended to the resultant file.

When a descriptor is loaded the **version** of the file is read from disk file.



## Serializers

**OSSerializer** and **ISSerializer**, Serializers is the core class of the system, serializers calls Pong::serializer::serialize functions and saves or load data into a file.

Serializers needs an **std::ofstream** (**OSSerializer**) or an **std::ifstream** (**ISSerializer**) objects.

