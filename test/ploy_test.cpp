//
// Created by 赵启明 on 2023/6/28.
//
#include "debug.h"
#include "elem_param_factory.h"
#include "nbtheory.h"
#include "poly.h"
#include "logger.h"
#include <gtest/gtest.h>

namespace zhejiangfhe {
    TEST(PolyTest, ConstructCopyAndAssignment) {
        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;
        uint32_t m = 8;
        uint32_t bits = 20;

        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, bits);

        BPoly poly = BPoly(params);
        poly = {"1", "2", "0", "1"};

        EXPECT_EQ(poly.GetLength(), m / 2) << "Polynomial init with string list failed";

        BPoly poly2 = BPoly(params);
        poly2 = {1, 2, 0, 1};
        EXPECT_EQ(poly.GetLength(), m / 2) << "Polynomial init with int list failed";

        EXPECT_EQ(poly, poly2) << "Polynomials set with string list and int "
                                  "list should be same.";

        BPoly poly3 = BPoly(poly);
        EXPECT_EQ(poly3, poly2) << "The new copied polynomial should be equal to "
                                   "polynomial with same params and coefficients";
        EXPECT_EQ(poly3, poly) << "The new copied polynomial should be the same "
                                  "with the originalg one.";

        BPoly poly4 = poly;
        EXPECT_EQ(poly4, poly2) << "The new assignment polynomial should be "
                                   "equal to polynomial with "
                                   "same params and coefficients";
        EXPECT_EQ(poly4, poly) << "The new assignment polynomial should be the "
                                  "same with the original one.";
    }

    TEST(PolyTest, AddingTwoPolynomials) {
        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;
        uint32_t m = 8;
        uint32_t bits = 20;

        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, bits);

        BPoly poly1(params);
        poly1 = {2, 3, 0, 1};

        BPoly poly2(params);
        poly2 = {1, 4, 5, 2};

        BPoly expectedPoly(params);
        expectedPoly = {3, 7, 5, 3};

        BPoly computedPoly = poly1.Add(poly2);

        EXPECT_EQ(computedPoly, expectedPoly) << "Addition operation on polynomials failed.";

        poly1.AddEq(poly2);
        EXPECT_EQ(poly1, expectedPoly) << "Addition operation in place on polynomials failed.";
    }

    TEST(PolyTest, SubtractPolynomial) {
        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;
        uint32_t m = 8;
        uint32_t bits = 20;

        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, bits);

        BPoly poly1(params);
        poly1 = {4, 4, 4, 4};

        BPoly poly2(params);
        poly2 = {1, 2, 3, 4};

        BPoly expectedPoly(params);
        expectedPoly = {3, 2, 1, 0};

        BPoly computedPoly(params);
        computedPoly = poly1.Sub(poly2);
        EXPECT_EQ(computedPoly, expectedPoly) << "Subtraction operation on polynomials failed.";

        poly1.SubEq(poly2);
        EXPECT_EQ(poly1, expectedPoly) << "Subtraction operation in place on polynomials failed.";
    }

    TEST(PolyTest, MultiplyScalar) {
        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;
        uint32_t m = 8;
        uint32_t bits = 20;

        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, bits);

        BPoly poly(params);
        poly = {1, 2, 3, 4};

        BPoly computedPoly = poly.MultiplyScalar(2);
        BPoly expectedPoly(params);
        expectedPoly = {2, 4, 6, 8};
        EXPECT_EQ(computedPoly, expectedPoly) << "MultiplyScalar operation on polynomials failed.";

        poly.MultiplyScalarEq(2);
        EXPECT_EQ(poly, expectedPoly) << "MultiplyScalar operation in place on polynomials failed.";
    }


    TEST(PolyTest, MultiplyForEvaluation) {
        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;
        uint32_t m = 8;
        uint32_t bits = 20;

        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, bits);

        BPoly poly1(params);
        poly1 = {4, 4, 4, 4};

        BPoly poly2(params);
        poly2 = {1, 2, 3, 4};

        BPoly expectedPoly(params);
        expectedPoly = {4, 8, 12, 16};

        BPoly computedPoly(params);
        computedPoly = poly1.MultiplyForEvaluation(poly2);
        EXPECT_EQ(computedPoly, expectedPoly)
                << "MultiplyForEvaluation operation on polynomials failed.";

        poly1.MultiplyForEvaluationEq(poly2);
        EXPECT_EQ(poly1, expectedPoly) << "MultiplyForEvaluation operation in "
                                          "place on polynomials failed.";
    }

    TEST(PolyTest, SetFormat) {
        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;

        uint32_t m = 8;
        uint32_t bits = 5;
        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, bits);

        BPoly coeffPoly(params, Format::COEFFICIENT);
        coeffPoly = {3, 0, 0, 0};

        BPoly evalPoly(params, Format::EVALUATION);
        evalPoly = {3, 3, 3, 3};

        EXPECT_NE(coeffPoly, evalPoly);

        BPoly tempPoly(coeffPoly);
        tempPoly.SetFormat(Format::EVALUATION);
        EXPECT_EQ(tempPoly, evalPoly) << "Convert values representation from coefficient format to "
                                         "evaluation format failed.";

        tempPoly.SetFormat(Format::COEFFICIENT);
        EXPECT_EQ(tempPoly, coeffPoly) << "Convert values representation from evaluation format to "
                                          "coefficient format failed.";
    }

    TEST(PolyTest, SwitchFormat) {

        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;

        uint32_t m = 16, nBits = 22;
        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, nBits);
        ZJTrace("modulus: {}, root: {}, order: {}",params->GetModulus(), params->GetRootOfUnity(),  params->GetCyclotomicOrder());

        BPoly coeffPoly(params, Format::COEFFICIENT);
        coeffPoly = {431, 3414, 1234, 7845, 2145, 7415, 5471, 8452};

        BPoly evalPoly(params, Format::EVALUATION);
        evalPoly = {1877267, 1022026, 3006168, 1772286, 1331622, 1039762, 869706, 1667670};

        BPoly poly(coeffPoly);

        poly.SwitchFormat();
        EXPECT_EQ(poly, evalPoly) << "Switch format from coefficient format to "
                                     "evaluation format failed";
        ZJTrace("Evalation poly: {}", poly);

        poly.SwitchFormat();
        EXPECT_EQ(poly, coeffPoly) << "Switch format from evaluation format to "
                                      "coefficient format failed";
        ZJTrace("Coefficient poly: {}", poly);
    }

    TEST(PolyTest, MultiplyPoly) {

        using BPoly = Poly<Vector<limbtype>>;
        using Params = typename BPoly::Params;

        uint32_t m = 8, nBits = 14;
        std::shared_ptr<Params> params = ElemParamFactory::GenElemParams<Params>(m, nBits);
        ZJTrace("modulus: {}, root: {}, order: {}", params->GetModulus(), params->GetRootOfUnity(), params->GetCyclotomicOrder());

        BPoly poly1(params, Format::COEFFICIENT);
        poly1 = {1, 2, 3, 4};

        BPoly poly2(params, Format::COEFFICIENT);
        poly2 = {4, 3, 2, 1};

        BPoly expectedPoly(params, Format::COEFFICIENT);
        expectedPoly = {16401, 0, 16, 30};

        poly1.MultiplyPolyEq(poly2);
        ZJTrace("Coefficient poly: {}", poly1);

        EXPECT_EQ(poly1, expectedPoly);
    }
}// namespace zhejiangfhe