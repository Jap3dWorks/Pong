---
description: serialization
---

# File Data

_`#include "Pong/filedata/serial_data"`_

file\_data is the serializer system, also includes serializable structs that can represent geometries and nodes.

_Needs real testing for write and for read data._

__

## serial\_data.h

Here is the the most high level functions,&#x20;

### Create serializer for custom objects

Override function&#x20;

```
template<typename Archive>
Pong::serializer::serialize(Archive& ar, Custom custom, Pong::serializer::Version version) {
   ar & custom.attr_1;
   ar * custom.attr_2;
}

```

### Create Struct data

You can use SERIALIZABLE, FIELD and IMPL\_SERIALIZE macros to create a default implement for structs.

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

IAssetDescription and OAssetDescription classes will define an structure to be serialized in a document.

Descriptors represents a set of elements like meshes, transforms and materials.

