-- This file is generated by holgen. Do not modify manually.
---@diagnostic disable: missing-return
---@class TestStructSingleElemContainerMeta
---@field singleElemStructs TestStructSingleElemMeta[]
---@field singleElemStructsWithId TestStructSingleElemWithIdMeta[]
TestStructSingleElemContainerMeta = {}

---@param elem TestStructSingleElemMeta
---@return TestStructSingleElemMeta
function TestStructSingleElemContainerMeta:AddSingleElemStruct(elem) end

---@param idx number
---@return TestStructSingleElemMeta
function TestStructSingleElemContainerMeta:GetSingleElemStruct(idx) end

---@param idx number
function TestStructSingleElemContainerMeta:DeleteSingleElemStruct(idx) end

---@return number
function TestStructSingleElemContainerMeta:GetSingleElemStructCount() end

---@param elem TestStructSingleElemWithIdMeta
---@return TestStructSingleElemWithIdMeta
function TestStructSingleElemContainerMeta:AddSingleElemStructWithId(elem) end

---@param idx number
---@return TestStructSingleElemWithIdMeta
function TestStructSingleElemContainerMeta:GetSingleElemStructWithId(idx) end

---@return number
function TestStructSingleElemContainerMeta:GetSingleElemStructWithIdCount() end
