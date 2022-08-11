#include "DescriptorPool.hpp"

#include "Utilities.hpp"

namespace VE
{
	DescriptorPool::DescriptorPool(std::shared_ptr<Device> device, const uint32_t descriptorCount, const std::vector<DescriptorSpec>& descriptorSpecs, uint32_t maxSets)
		: m_Device(device), m_DescriptorPool(VK_NULL_HANDLE), m_MaxSets(maxSets)
	{
		m_DescriptorsSizes.resize(descriptorCount);
		CreateDescriptorPool(descriptorSpecs);
	}

	DescriptorPool::~DescriptorPool()
	{
		if (m_DescriptorPool)
		{
			vkDestroyDescriptorPool(m_Device->GetVkDevice(), m_DescriptorPool, nullptr);
		}
	}

	void DescriptorPool::CreateDescriptorPool(const std::vector<DescriptorSpec>& descriptorSpecs)
	{
		size_t index = 0;
		for (VkDescriptorPoolSize& poolSize : m_DescriptorsSizes)
		{
			poolSize.type = descriptorSpecs[index].type;
			poolSize.descriptorCount = descriptorSpecs[index].count;

			index++;
		}

		VkDescriptorPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolCreateInfo.maxSets = m_MaxSets;
		poolCreateInfo.poolSizeCount = static_cast<uint32_t>(m_DescriptorsSizes.size());
		poolCreateInfo.pPoolSizes = m_DescriptorsSizes.data();

		VK_CHECK(vkCreateDescriptorPool(m_Device->GetVkDevice(), &poolCreateInfo, nullptr, &m_DescriptorPool))
	}
}
