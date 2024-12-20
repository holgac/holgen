-- This file is generated by holgen. Do not modify manually.
---@diagnostic disable: missing-return
---@class TestContainerVector
---@field innerStructsWithId TestContainerInnerStructWithId[]
---@field innerStructsNoId TestContainerInnerStructNoId[]
---@field stringContainer string[]
---@field unsignedContainer number[]
TestContainerVector = {}

---@param key string
---@return TestContainerInnerStructWithId
function TestContainerVector:GetInnerStructWithIdFromName(key) end

---@param key string
---@return TestContainerInnerStructNoId
function TestContainerVector:GetInnerStructNoIdFromName(key) end

---@param elem TestContainerInnerStructWithId
---@return TestContainerInnerStructWithId
function TestContainerVector:AddInnerStructWithId(elem) end

---@param elem TestContainerInnerStructNoId
---@return TestContainerInnerStructNoId
function TestContainerVector:AddInnerStructNoId(elem) end

---@param elem string
---@return string
function TestContainerVector:AddStringElem(elem) end

---@param elem number
---@return number
function TestContainerVector:AddUnsignedElem(elem) end

---@param idx number
---@return TestContainerInnerStructWithId
function TestContainerVector:GetInnerStructWithId(idx) end

---@param idx number
---@return TestContainerInnerStructNoId
function TestContainerVector:GetInnerStructNoId(idx) end

---@param idx number
---@return string
function TestContainerVector:GetStringElem(idx) end

---@param idx number
---@return number
function TestContainerVector:GetUnsignedElem(idx) end

---@param idx number
function TestContainerVector:DeleteInnerStructWithId(idx) end

---@param idx number
function TestContainerVector:DeleteInnerStructNoId(idx) end

---@param idx number
function TestContainerVector:DeleteStringElem(idx) end

---@param idx number
function TestContainerVector:DeleteUnsignedElem(idx) end

---@return number
function TestContainerVector:GetInnerStructWithIdCount() end

---@return number
function TestContainerVector:GetInnerStructNoIdCount() end

---@return number
function TestContainerVector:GetStringElemCount() end

---@return number
function TestContainerVector:GetUnsignedElemCount() end
