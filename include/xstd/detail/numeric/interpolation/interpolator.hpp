/**
 * \file       interpolator.hpp
 * \author     Bryan Flynt
 * \date       Mar 04, 2021
 * \copyright  Copyright (C) 2021 Bryan Flynt - All Rights Reserved
 */
#pragma once


#include <memory>


class Interpolator {

    using Kernel = ;
    using Polynomial = ;

    using KernelPtr     = std::shared_ptr<Kernel>;
    using PolynomialPtr = std::shared_ptr<Polynomial>;


    void setKernel(const KernelPtr& kernel);

    void setPolynomial(const PolynomialPtr& polynomial);

    void setLocations(const PointList& source_points, const PointList& target_points);

    void getWeights() const;


private:
    KernelPtr     kernel_ptr_;
    PolynomialPtr polynomial_ptr_;

    Matrix        source_basis_matrix_;
    Matrix        target_basis_matrix_;
    Matrix        source_poly_matrix_;
    Matrix        target_poly_matrix_;
    Matrix        weight_matrix_;  // target_basis_matrix_ * inv(source_basis_matrix_)
};


void 
Interpolator::setKernel(const KernelPtr& kernel){
    kernel_ptr_ = kernel;
}

void 
Interpolator::setPolynomial(const PolynomialPtr& polynomial){
    polynomial_ptr_ = polynomial;
}

void 
void setLocations(const PointList& source_points, const PointList& target_points);
    source_basis_matrix_ = kernel_ptr_->calc_matrix(source_points,source_points);
    source_poly_matrix_  = polynomial_ptr_->calc_matrix(source_points);
    target_basis_matrix_ = kernel_ptr_->calc_matrix(target_points,source_points);
    target_poly_matrix_  = polynomial_ptr_->calc_matrix(target_points);
}



