#include "Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"
#include "glm/gtx/transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <chrono>

namespace std
{
	template<> struct hash<VE::Vertex> {
		size_t operator()(VE::Vertex const& vertex) const
		{
			return ((hash<glm::vec3>()(vertex.position) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoords) << 1);
		}
	};
}

namespace VE
{
	Model::Model(Device* device, std::string_view modelPath)
		:	m_Device(device), m_ModelPath(modelPath.data()),
			m_DescriptorSet(device), m_Transform(glm::mat4(1.0f))
	{
		m_DescriptorSet.Create();
		m_DescriptorSet.SetTexture(0, 1, "D:\\OpenGL Projects\\VulkanEngine\\Res\\Textures\\viking_room.png");
		m_DescriptorSet.SetTexture(1, 1, "D:\\OpenGL Projects\\VulkanEngine\\Res\\Textures\\viking_room.png");

		LoadModel();
	}

	void Model::Bind(VkCommandBuffer commandBuffer) const
	{
		m_VertexBuffer->BindBuffer(commandBuffer);
		m_IndexBuffer->BindBuffer(commandBuffer);
	}

	void Model::Draw(VkCommandBuffer commandBuffer) const
	{
		vkCmdDrawIndexed(commandBuffer, m_IndexBuffer->GetDataCount(), 1, 0, 0, 0);
	}

	void Model::LoadModel()
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		std::unordered_map<Vertex, uint32_t> uniqueVertices{};
		std::vector<Vertex> vertices;
		std::vector<uint16_t> indices;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_ModelPath))
		{
			throw std::runtime_error(warn + err);
		}

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				vertex.position =
				{
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.texCoords = 
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.color = 
				{
					1.0f, 1.0f, 1.0f 
				};

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}

		m_VertexBuffer = std::make_unique<VertexBuffer>(m_Device, vertices.size() * sizeof(Vertex), vertices.data());
		m_IndexBuffer = std::make_unique<IndexBuffer>(m_Device, indices.size() * sizeof(uint16_t), indices.data());
	}

	void Model::UpdateDescriptors(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const uint32_t currentFrame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		m_GUBO.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_GUBO.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		m_GUBO.proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 10.0f);
		m_GUBO.proj[1][1] *= -1;

		m_DescriptorSet.UpdateBuffer(currentFrame, 0, &m_GUBO, sizeof(m_GUBO));
		m_DescriptorSet.UpdateImage(currentFrame, 1);
		m_DescriptorSet.Bind(commandBuffer, pipelineLayout, currentFrame);
	}
}