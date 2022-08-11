#pragma once

#include "Device.hpp"
#include "DescriptorSpec.hpp"

#include <memory>
#include <vector>

namespace VE
{
	class DescriptorPool
	{
	public:
		DescriptorPool(std::shared_ptr<Device> device, const uint32_t descriptorCount, const std::vector<DescriptorSpec>& descriptorSpecs, uint32_t maxSets);
		~DescriptorPool();
	public:
		inline VkDescriptorPool GetVkPool() const { return m_DescriptorPool; }
	private:
		void CreateDescriptorPool(const std::vector<DescriptorSpec>& descriptorSpecs);
	private:
		std::shared_ptr<Device>				m_Device;
		std::vector<VkDescriptorPoolSize>	m_DescriptorsSizes;
		VkDescriptorPool					m_DescriptorPool;
		uint32_t							m_MaxSets;
	};
}

