//
// Created by 赵启明 on 2023/6/26.
//

#include "poly.h"

namespace zhejiangfhe {
    template <typename VecType>
    Poly<VecType>::Poly() : value(nullptr), format(Format::EVALUATION) {}


    template <typename VecType>
    Poly<VecType>::Poly(const Poly::Params params, Format format, bool initializeElementToZero)
        : value(nullptr), format(format) {
        params = params;
        if (initializeElementToZero) {
            this->SetValueToZero();
        }
    }


    template <typename VecType>
    Poly<VecType>::Poly(const Poly& element, Poly::Params)
        : format(element.format), params(element.params) {

        if (element.values == nullptr) {
            value = nullptr;
        }
        else {
            value = VecType(*element.value);
        }
    }

    template <typename VecType>
    bool Poly<VecType>::IsEmpty() const {
        if (value == nullptr)
            return true;
        return GetLength() == 0;
    }


    template <typename VecType>
    uint32_t Poly<VecType>::GetLength() const {
        if (value  == 0)
            ZJFHE_THROW(NotAvailableError, "No values in Poly");
        return value->GetLength();
    }

    template <typename VecType>
    const Poly<VecType>& Poly<VecType>::operator=(std::initializer_list<std::string> rhs) {
        static Integer ZERO(0);
        uint32_t len = rhs.size();
        if (!IsEmpty()) {
            uint32_t vectorLength = this->values->GetLength();

            for (uint32_t j = 0; j < vectorLength; ++j) {  // loops within a tower
                if (j < len) {
                    value->operator[](j) = *(rhs.begin() + j);
                } else {
                    value->operator[](j) = ZERO;
                }
            }
        } else {
            VecType temp(params->GetRingDimension());
            temp.SetModulus(params->GetModulus());
            temp = rhs;
            this->SetValue(std::move(temp), format);
        }
        return *this;
    }

    template <typename VecType>
    const Poly<VecType>& Poly<VecType>::operator=(std::vector<int64_t> rhs) {
        static Integer ZERO(0);
        uint32_t len = rhs.size();
        if (!IsEmpty()) {
            uint32_t vectorLength = this->values->GetLength();

            for (uint32_t j = 0; j < vectorLength; ++j) {  // loops within a tower
                if (j < len) {
                    Integer tempBI;
                    uint64_t tempInteger;
                    if (*(rhs.begin() + j) < 0) {
                        tempInteger = -*(rhs.begin() + j);
                        tempBI      = params->GetModulus() - Integer(tempInteger);
                    } else {
                        tempInteger = *(rhs.begin() + j);
                        tempBI      = Integer(tempInteger);
                    }
                    operator[](j) = tempBI;
                } else {
                    operator[](j) = ZERO;
                }
            }
        } else {
            uint32_t vectorLength = params->GetCyclotomicOrder() / 2;
            VecType temp(vectorLength);
            temp.SetModulus(params->GetModulus());
            for (uint32_t j = 0; j < vectorLength; ++j) {  // loops within a tower
                if (j < len) {
                    Integer tempBI;
                    uint64_t tempInteger;
                    if (*(rhs.begin() + j) < 0) {
                        tempInteger = -*(rhs.begin() + j);
                        tempBI      = params->GetModulus() - Integer(tempInteger);
                    } else {
                        tempInteger = *(rhs.begin() + j);
                        tempBI      = Integer(tempInteger);
                    }
                    temp.operator[](j) = tempBI;
                }
                else {
                    temp.operator[](j) = ZERO;
                }
            }
            this->SetValues(std::move(temp), format);
        }
        format = Format::COEFFICIENT;
        return *this;
    }

    template <typename VecType>
    const Poly<VecType>& Poly<VecType>::operator=(std::vector<int32_t> rhs) {
        static Integer ZERO(0);
        uint32_t len = rhs.size();
        if (!IsEmpty()) {
            uint32_t vectorLength = this->values->GetLength();

            for (uint32_t j = 0; j < vectorLength; ++j) {  // loops within a tower
                if (j < len) {
                    Integer tempBI;
                    uint64_t tempInteger;
                    if (*(rhs.begin() + j) < 0) {
                        tempInteger = -*(rhs.begin() + j);
                        tempBI      = params->GetModulus() - Integer(tempInteger);
                    } else {
                        tempInteger = *(rhs.begin() + j);
                        tempBI      = Integer(tempInteger);
                    }
                    operator[](j) = tempBI;
                } else {
                    operator[](j) = ZERO;
                }
            }
        } else {
            uint32_t vectorLength = params->GetCyclotomicOrder() / 2;
            VecType temp(vectorLength);
            temp.SetModulus(params->GetModulus());
            for (uint32_t j = 0; j < vectorLength; ++j) {  // loops within a tower
                if (j < len) {
                    Integer tempBI;
                    uint64_t tempInteger;
                    if (*(rhs.begin() + j) < 0) {
                        tempInteger = -*(rhs.begin() + j);
                        tempBI      = params->GetModulus() - Integer(tempInteger);
                    } else {
                        tempInteger = *(rhs.begin() + j);
                        tempBI      = Integer(tempInteger);
                    }
                    temp.operator[](j) = tempBI;
                } else {
                    temp.operator[](j) = ZERO;
                }
            }
            this->SetValue(std::move(temp), format);
        }
        format = Format::COEFFICIENT;
        return *this;
    }

    template <typename VecType>
    const Poly<VecType>& Poly<VecType>::operator=(Poly&& rhs) {
        if (this != &rhs) {
            value = std::move(rhs.value);
            params = rhs.params;
            format = rhs.format;
        }

        return *this;
    }


    template <typename VecType>
    const Poly<VecType>& Poly<VecType>::operator=(uint64_t val) {
        format = Format::EVALUATION;
        if (value == nullptr) {
            value = VecType(params->GetRingDimension(), params->GetModulus());
        }
        for (size_t i = 0; i < value->GetLength(); ++i) {
            this->operator[](i) = Integer(val);
        }
        return *this;
    }


    template <typename VecType>
    void Poly<VecType>::SetValue(const VecType& value, Format format) {
        if (params->GetRootOfUnity() == Integer(0)) {
            ZJFHE_THROW(TypeException, "Polynomial has a 0 root of unity");
        }
        if (params->GetRingDimension() != value.GetLength() || params->GetModulus() != value.GetModulus()) {
            ZJFHE_THROW(TypeException, "Parameter mismatch on SetValues for Polynomial");
        }
        this->value = value;
        this->format = format;
    }


    template <typename VecType>
    void Poly<VecType>::SetValueToZero() {
        value = VecType(params->GetRingDimension(), params->GetModulus());
    }

    template <typename VecType>
    void Poly<VecType>::SetValuesToMax() {
        Integer max = params->GetModulus() - Integer(1);
        uint32_t size  = params->GetRingDimension();
        value    = VecType(params->GetRingDimension(), params->GetModulus());
        for (uint32_t i = 0; i < size; i++) {
            value->operator[](i) = Integer(max);
        }
    }


    template <typename VecType>
    typename Poly<VecType>::Integer& Poly<VecType>::at(uint32_t i) {
        if (value == 0)
            ZJFHE_THROW(NotAvailableError, "No values in Poly");
        return value->at(i);
    }

    template <typename VecType>
    const typename Poly<VecType>::Integer& Poly<VecType>::at(uint32_t i) const {
        if (value == 0)
            ZJFHE_THROW(NotAvailableError, "No values in PolyImpl");
        return value->at(i);
    }

    template <typename VecType>
    typename Poly<VecType>::Integer& Poly<VecType>::operator[](uint32_t i) {
        return (*value)[i];
    }

    template <typename VecType>
    const typename Poly<VecType>::Integer& Poly<VecType>::operator[](uint32_t i) const {
        return (*value)[i];
    }
}
