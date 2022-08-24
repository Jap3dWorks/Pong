---
description: namespace Pong::serializer
---

# File Data

file\_data is the serializer system, also includes serializable structs that can represent geometries and nodes.

## serial\_data.h

_`#include "Pong/file_data/serial_data"`_

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

## Serializers

**OSSerializer** and **ISSerializer** classes are the interface to serialize a [Descriptor](./#descriptors).

Osserializer::operator<< saves a descriptor into a file, ISSerializer::operator>> loads a file into a descriptor.

Serializers uses the function Pong::serializer::serialize for a correct serialization.

## Descriptors

**IAssetDescriptor** and **OAssetDescriptor** classes will define an asset structure to be serialized in a document.

**IMapDescriptor** and **IMapDescriptor** classes will define a map structure to be serialized in a document.

Descriptors represents a set of elements like actors, meshes and materials.

Descriptors should be registered using **REG\_DESCRIPTOR(class, version)** macro.

```cpp
class OAssetDescriptor;
REG_DESCRIPTOR(OAssetDescriptor, 1);
```

### Output descriptors

When a Descriptor is registered can be serialized into a file using **OSerializer::operator<<**.&#x20;

To serialize a descriptor into a file.

<pre class="language-cpp"><code class="lang-cpp"><strong>auto os = std::ofstream(file_name, std::ofstream::binary);
</strong><strong>Pong::serializer::OSSerializer(os) &#x3C;&#x3C; descriptor;</strong></code></pre>

Also you can use save\_file to serialize a descriptor into a file.

```cpp
Pong::serializer::save_file(OAssetDescriptor descriptor, const char* file_name);
```

### Input Descriptors

To load a file into a descriptor

<pre><code><strong>auto istream = std::ifstream(file_name, std::ofstream::binary);
</strong><strong>Pong::serializer::ISSerializer(istream) >> descriptor;</strong></code></pre>

### Version

When a descriptor is saved its **version** is appended to the resultant file.

When a descriptor is loaded the **version** of the file is read from disk file.



