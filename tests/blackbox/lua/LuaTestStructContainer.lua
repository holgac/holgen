-- This file is generated by holgen. Do not modify manually.
---@diagnostic disable: missing-return
---@class LuaTestStructContainerMeta
---@field testVector number[]
---@field testMap table<string, number>
LuaTestStructContainerMeta = {}

---@param elem number
---@return number
function LuaTestStructContainerMeta:AddTestVectorElem(elem) end

---@param idx number
---@return number
function LuaTestStructContainerMeta:GetTestVectorElem(idx) end

---@param idx number
function LuaTestStructContainerMeta:DeleteTestVectorElem(idx) end

---@return number
function LuaTestStructContainerMeta:GetTestVectorElemCount() end