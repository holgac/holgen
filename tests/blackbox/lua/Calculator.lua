-- This file is generated by holgen. Do not modify manually.
---@diagnostic disable: missing-return
---@class CalculatorMeta
---@field curVal NumberMeta
CalculatorMeta = {}

---@param val number
---@return number
function CalculatorMeta:Add(val) end

---@param val NumberMeta
---@return NumberMeta
function CalculatorMeta:Subtract(val) end
---@type table<string, fun(val: NumberMeta): NumberMeta>
Ops = nil

---@param lhs number
---@param rhs number
---@return number
function CalculatorMeta:SubtractThenMultiply(lhs, rhs) end