// Generated code

#if !DEBUG
#include "Shaders/ShaderProvider/ShaderProvider.h"
const string ShaderProvider::UnpackCode(const uint32_t& h) {
switch (h) {
case 1939412094: return "#version 460 core\nin vec2 fragmentTextureCoords;uniform sampler2D dataTexture;out vec4 color;void main()\n{\ncolor=texture(dataTexture, fragmentTextureCoords);}\n";
case 776390504: return "#version 460 core\nin vec4 position;in vec2 vertexTextureCoords;out vec2 fragmentTextureCoords;void main()\n{\ngl_Position=position; fragmentTextureCoords=vertexTextureCoords;}\n";
case 3759396228: return "#version 460 core\nlayout(local_size_x=8, local_size_y=8, local_size_z=1) in;struct SpeciesSettings\n{\nvec4 mask; vec4 mainColor; vec4 edgeColor; float moveSpeed; float turnSpeed; int sensorSize; float sensorOffset; float sensorAngleDegrees; float trailWeight;};uniform uint colonySize;layout(std430) restrict readonly buffer colonySettings\n{\nSpeciesSettings Colony[];};layout(rgba32f) restrict readonly uniform image2D trailTexture;restrict writeonly uniform image2D displayTexture;uniform int height;uniform int width;void main()\n{\nivec2 position=ivec2(gl_GlobalInvocationID.xy); if (position.x >= width || position.y >= height)\nreturn; vec4 trailData=imageLoad(trailTexture, position); vec4 color=vec4(0); for (uint i=0; i < colonySize; i++)\n{\nSpeciesSettings species=Colony[i]; float trailIntensity=dot(trailData, species.mask); vec4 mixedColor=mix(species.edgeColor, species.mainColor, trailIntensity); color += mixedColor * trailIntensity; }\nimageStore(displayTexture, position, color);}\n";
case 2859397344: return "#version 460 core\nlayout(local_size_x=8, local_size_y=8, local_size_z=1) in;layout(rgba32f) restrict readonly uniform image2D source;layout(rgba32f) restrict writeonly uniform image2D target;void main()\n{\nivec2 position=ivec2(gl_GlobalInvocationID.xy); imageStore(target, position, imageLoad(source, position));}\n";
case 2410421611: return "#version 460 core\nlayout(local_size_x=8, local_size_y=8, local_size_z=1) in;uniform int width;uniform int height;uniform float decayRate;uniform float diffuseRate;restrict writeonly uniform image2D diffusedTrailTexture;layout(rgba32f) restrict readonly uniform image2D trailTexture;const float globalSpeed=0.02;vec4 GetAverageNeighborTrail(ivec2 position)\n{\nvec4 totalTrail=vec4(0); for (int offsetY=-1; offsetY < 2; offsetY++)\n{\nfor (int offsetX=-1; offsetX < 2; offsetX++)\n{\nivec2 samplePosition=ivec2(\nclamp(position.x + offsetX, 0, width - 1),\nclamp(position.y + offsetY, 0, height - 1)\n); totalTrail += imageLoad(trailTexture, samplePosition); }\n}\nreturn totalTrail / 9;}\nvoid main()\n{\nivec2 position=ivec2(gl_GlobalInvocationID.xy); if (position.x >= width || position.y >= height)\nreturn; vec4 trail=imageLoad(trailTexture, position); vec4 averageNeighborTrail=GetAverageNeighborTrail(position); float diffuseWeight=clamp(diffuseRate * globalSpeed, 0.0, 1.0); vec4 mergedTrail=(1 - diffuseWeight) * trail + diffuseWeight * averageNeighborTrail; vec4 decayedTrail=max(vec4(0), mergedTrail - decayRate * globalSpeed); imageStore(diffusedTrailTexture, position, decayedTrail);}\n";
case 1178242111: return "#version 460 core\nlayout(local_size_x=16, local_size_y=1, local_size_z=1) in;struct SlimeCell\n{\nvec2 position; float angle; int speciesIndex;};struct SpeciesSettings\n{\nvec4 mask; vec4 mainColor; vec4 edgeColor; float moveSpeed; float turnSpeed; int sensorSize; float sensorOffset; float sensorAngleDegrees; float trailWeight;};layout(std430) restrict readonly buffer colonySettings\n{\nSpeciesSettings Colony[];};uniform uint cellCount;layout(std430) restrict buffer slimeCells\n{\nSlimeCell Cells[];};uniform int width;uniform int height;layout(rgba32f) restrict uniform image2D trailTexture;uniform uint seed;uniform uint userSeed;const float globalSpeed=0.02;const float tau=6.2831853072;uint Random(uint state)\n{\nstate ^= 2747636419; state *= 2654435769; state ^= state >> 16; state *= userSeed + 1; state *= 2654435769; state ^= state >> 16; state *= 2654435769; return state;}\nfloat NormalizeUnsignedInt(uint value)\n{\nreturn value / 4294967295.0;}\nfloat SenseTrail(SlimeCell cell, float sensorAngleOffset)\n{\nSpeciesSettings species=Colony[cell.speciesIndex]; int sensorSize=species.sensorSize; float sensorAngle=cell.angle + sensorAngleOffset; vec2 sensorDirection=vec2(cos(sensorAngle), sin(sensorAngle)); vec2 sensorPosition=sensorDirection * species.sensorOffset + cell.position; ivec4 senseWeight=ivec4(species.mask * 2 - 1); float senseTotal=0; for (int offsetY=-sensorSize; offsetY <= sensorSize; offsetY++)\n{\nfor (int offsetX=-sensorSize; offsetX <= sensorSize; offsetX++)\n{\nivec2 samplePosition=ivec2(\nclamp(int(sensorPosition.x) + offsetX, 0, width - 1),\nclamp(int(sensorPosition.y) + offsetY, 0, height - 1)\n); vec4 trailData=imageLoad(trailTexture, samplePosition); senseTotal += dot(senseWeight, trailData); }\n}\nreturn senseTotal;}\nvoid SteerCell(uint cellId, uint random)\n{\nSlimeCell cell=Cells[cellId]; SpeciesSettings species=Colony[cell.speciesIndex]; float steerStrength=NormalizeUnsignedInt(random); float turnSpeed=species.turnSpeed * tau * globalSpeed; float forwardWeight=SenseTrail(cell, 0); float sensorAngleRad=radians(species.sensorAngleDegrees); float rightWeight=SenseTrail(cell, -sensorAngleRad); float leftWeight=SenseTrail(cell, sensorAngleRad); if (forwardWeight <= leftWeight || forwardWeight <= rightWeight)\n{\nif (forwardWeight < leftWeight && forwardWeight < rightWeight) // Turn randomly\nCells[cellId].angle += (steerStrength - 0.5) * 2 * turnSpeed; else if (rightWeight > leftWeight) // Turn right\nCells[cellId].angle -= steerStrength * turnSpeed; else if (leftWeight > rightWeight) // Turn left\nCells[cellId].angle += steerStrength * turnSpeed; }\n}\nvoid MoveCell(uint cellId, uint random)\n{\nSlimeCell cell=Cells[cellId]; SpeciesSettings species=Colony[cell.speciesIndex]; vec2 direction=vec2(cos(cell.angle), sin(cell.angle)); vec2 newPos=cell.position + direction * species.moveSpeed * globalSpeed; // Bounce on boundaries\nif (newPos.x < 0 || newPos.x >= width || newPos.y < 0 || newPos.y >= height)\n{\nfloat randomAngle=NormalizeUnsignedInt(random) * tau; Cells[cellId].angle=randomAngle; newPos.x=clamp(newPos.x, 0, width - 1); newPos.y=clamp(newPos.y, 0, height - 1); }\nelse\n{\nivec2 newTrailCoords=ivec2(newPos); vec4 oldTrailData=imageLoad(trailTexture, newTrailCoords); vec4 newTrailData=min(vec4(1),\noldTrailData + species.mask * species.trailWeight * globalSpeed\n); imageStore(trailTexture, newTrailCoords, newTrailData); }\nCells[cellId].position=newPos;}\nvoid main()\n{\nuint cellId=gl_GlobalInvocationID.x; if (cellId >= cellCount)\nreturn; vec2 position=Cells[cellId].position; uint positionId=uint(position.y * width + position.x); uint random=Random(positionId + Random(seed * 100000 + cellId)); SteerCell(cellId, random); MoveCell(cellId, Random(random));}\n";
default: return ""; }}
#endif
