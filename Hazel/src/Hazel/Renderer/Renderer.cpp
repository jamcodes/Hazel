#include "Renderer.h"

namespace Hazel {

void Renderer::beginScene()
{

}

void Renderer::endScene()
{

}

void Renderer::submit(VertexArray const& vertex_array)
{
    vertex_array.bind();
    RenderCommand::drawIndexed(vertex_array);
}


}  // namespace Hazel
