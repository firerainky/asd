//
// Created by 赵启明 on 2023/6/28.
//
#include "elem_param_factory.h"
#include "nbtheory.h"
#include "poly.h"
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

        EXPECT_EQ(poly, poly2) << "Polynomials set with string list and int list should be same.";

        BPoly poly3 = BPoly(poly);
        EXPECT_EQ(poly3, poly2) << "The new copied polynomial should be equal to polynomial with same params and coefficients";
        EXPECT_EQ(poly3, poly) << "The new copied polynomial should be the same with the originalg one.";

        BPoly poly4 = poly;
        EXPECT_EQ(poly4, poly2) << "The new assignment polynomial should be equal to polynomial with same params and coefficients";
        EXPECT_EQ(poly4, poly) << "The new assignment polynomial should be the same with the original one.";
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
}// namespace zhejiangfhe