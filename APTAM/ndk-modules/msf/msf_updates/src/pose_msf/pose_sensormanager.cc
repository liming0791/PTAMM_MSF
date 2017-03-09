/*
 * Copyright (C) 2012-2013 Simon Lynen, ASL, ETH Zurich, Switzerland
 * You can contact the author at <slynen at ethz dot ch>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef POSE_MEASUREMENTMANAGER_H
#define POSE_MEASUREMENTMANAGER_H

#include <stdio.h>
#include <iostream>
#include <fstream>

#include <msf_timing/Timer.h>
#include <msf_core/msf_core.h>
#include <msf_core/msf_sensormanager.h>
#include <msf_core/msf_IMUHandler.h>
#include "msf_statedef.hpp"
#include "PoseSensorConfig.h"
#include <msf_updates/pose_sensor_handler/pose_sensorhandler.h>
#include <msf_updates/pose_sensor_handler/pose_measurement.h>


namespace msf_pose_sensor {

    typedef void (* Callback)(const Eigen::Matrix<double, 3, 1>&, 
            const Eigen::Matrix<double, 4, 1>&);

    class PoseSensorManager : 
        public msf_core::MSF_SensorManager<msf_updates::EKFState> 
    {

        typedef PoseSensorHandler \
            <msf_updates::pose_measurement::PoseMeasurement<>, \
            PoseSensorManager>  PoseSensorHandler_T;
        friend class PoseSensorHandler
            <msf_updates::pose_measurement::PoseMeasurement<>,
            PoseSensorManager> ;

        public:

        typedef msf_updates::EKFState EKFState_T;
        typedef EKFState_T::StateSequence_T StateSequence_T;
        typedef EKFState_T::StateDefinition_T StateDefinition_T;

        PoseSensorManager(): noiseAcc(0.3), noiseAccbias(0.03), noiseGyr(0.005), noiseGyrbias(0.0005) 
        {
            printf("\n========Config========\n");
            printf("noiseAcc: %f\n", noiseAcc);
            printf("noiseAccbias: %f\n", noiseAccbias);
            printf("noiseGyr: %f\n", noiseGyr);
            printf("noiseGyrbias: %f\n", noiseGyrbias);
            printf("pose_noise_meas_p: %f\n", config_.pose_noise_meas_p);
            printf("pose_noise_meas_q: %f\n", config_.pose_noise_meas_q);
            printf("pose_delay: %f\n\n", config_.pose_delay);

            bool distortmeas = false;  ///< Distort the pose measurements.

            imu_handler_.reset(new msf_core::IMUHandler
                    <msf_updates::EKFState>
                    (*this, "msf_core", "imu_handler"));

            pose_handler_.reset(new PoseSensorHandler_T
                    (*this, "", "pose_sensor", distortmeas));

            pose_handler_->SetNoises(config_.pose_noise_meas_p,
                    config_.pose_noise_meas_q);
            pose_handler_->SetDelay(config_.pose_delay);

            AddHandler(pose_handler_);

            callback_ = NULL;

            //InitScale(1.0);

        }

        PoseSensorManager(std::string config_file_name): noiseAcc(0.08), noiseAccbias(0.008), noiseGyr(0.02), noiseGyrbias(0.002) 
        {

            // read params from file
            std::ifstream config_file(config_file_name);
            if (config_file.is_open()) {
                config_file >> noiseAcc >> noiseAccbias >> noiseGyr >> noiseGyrbias; 
                config_file >> config_.pose_noise_meas_p >> config_.pose_noise_meas_q;
                config_file >> config_.pose_delay;

                printf("\n========Config========\n");
                printf("noiseAcc: %f\n", noiseAcc);
                printf("noiseAccbias: %f\n", noiseAccbias);
                printf("noiseGyr: %f\n", noiseGyr);
                printf("noiseGyrbias: %f\n", noiseGyrbias);
                printf("pose_noise_meas_p: %f\n", config_.pose_noise_meas_p);
                printf("pose_noise_meas_q: %f\n", config_.pose_noise_meas_q);
                printf("pose_delay: %f\n\n", config_.pose_delay);
            } else {
                printf("\n========Config========\n");
                printf("Warning: open configure file failed: %s\n\n", config_file_name.c_str());
            }
            
            bool distortmeas = false;  ///< Distort the pose measurements.

            imu_handler_.reset(new msf_core::IMUHandler
                    <msf_updates::EKFState>
                    (*this, "msf_core", "imu_handler"));

            pose_handler_.reset(new PoseSensorHandler_T
                    (*this, "", "pose_sensor", distortmeas));

            pose_handler_->SetNoises(config_.pose_noise_meas_p,
                    config_.pose_noise_meas_q);
            pose_handler_->SetDelay(config_.pose_delay);

            AddHandler(pose_handler_);

            callback_ = NULL;

            //InitScale(1.0);

        }
        ~PoseSensorManager() { }

        void Set_q_wv(double q0, double q1, double q2, double q3) {
            _q_wv.w() = q0;
            _q_wv.x() = q1;
            _q_wv.y() = q2;
            _q_wv.z() = q3;
        }

        void Set_q_ic(double q0, double q1, double q2, double q3) {
            _q_ic.w() = q0;
            _q_ic.x() = q1;
            _q_ic.y() = q2;
            _q_ic.z() = q3;
        }

        bool InitScale(float scale) 
        {
            printf("Initialize filter with scale %f\n", scale);
            Init(scale);
            return true;
        }

        const PoseSensorConfig& Getcfg() 
        {
            return config_;
        }

        void ProcessIMU(const msf_core::Vector3& linear_acceleration,
                const msf_core::Vector3& angular_velocity,
                const double& msg_stamp, size_t msg_seq) 
        { 
            imu_handler_->ProcessIMU(linear_acceleration,
                    angular_velocity, 
                    msg_stamp, msg_seq); 
        }

        void ProcessPose(const boost::shared_ptr
                < msf_updates::pose_measurement::
                PoseWithCovarianceStamped const>& msg) 
        {
            pose_handler_->ProcessPoseMeasurement(msg);
        }

        void PublishStateInitial
            (const shared_ptr< EKFState_T >& state) const 
            {
                printf("TODO::publishstateinitial, currently do nothing\n");
                return;
            }

        void PublishStateAfterPropagation
            (const shared_ptr<EKFState_T>& state) const 
            {
                Eigen::Matrix<double, 3, 1> position;
                Eigen::Matrix<double, 4, 1> rotation;
                state->ToPoseVectors(position, rotation);
                if (callback_) {
                    callback_(position, rotation);
                    std::cout << "Callback after Predict" << std::endl << std::endl;
                }
            }

        void PublishStateAfterUpdate
            (const shared_ptr<EKFState_T>& state) const 
            {
                Eigen::Matrix<double, 3, 1> position;
                Eigen::Matrix<double, 4, 1> rotation;
                state->ToPoseVectors(position, rotation);
                if (callback_) {
                    callback_(position, rotation);
                    std::cout << "Callback after Update" << std::endl << std::endl;
                }
            }

        void SetCallback(Callback F) 
        {
            callback_ = F;
        }

        private:

        shared_ptr<msf_core::IMUHandler<msf_updates::EKFState> > 
            imu_handler_;
        shared_ptr<PoseSensorHandler_T> pose_handler_;
        PoseSensorConfig config_;
        Callback callback_;

        Eigen::Quaternion<double> _q_wv;
        Eigen::Quaternion<double> _q_ic;

        /// Minimum initialization height. 
        /// If a abs(height) is smaller than this value, 
        /// no initialization is performed.
        static constexpr double MIN_INITIALIZATION_HEIGHT = 0.01;

        
        void Init(double scale) const 
        {
            Eigen::Matrix<double, 3, 1> 
                p, v, b_w, b_a, g, w_m, a_m, p_ic, p_vc, p_wv;
            Eigen::Quaternion<double> q, q_wv, q_ic, q_cv;
            msf_core::MSF_Core<EKFState_T>::ErrorStateCov P;

            // init values
            g << 0, 0, 9.81;	        /// Gravity.
            b_w << 0, 0, 0;		/// Bias gyroscopes.
            b_a << 0, 0, 0;		/// Bias accelerometer.

            v << 0, 0, 0;			/// Robot velocity (IMU centered).
            w_m << 0, 0, 0;		/// Initial angular velocity.

            q_wv.setIdentity();  // Vision-world rotation drift.
            p_wv.setZero();      // Vision-world position drift.

            q_wv.w() = _q_wv.w();
            q_wv.x() = _q_wv.x();
            q_wv.y() = _q_wv.y();
            q_wv.z() = _q_wv.z();

            P.setZero();  // Error state covariance; 
            // if zero, a default initialization 
            // in msf_core is used

            p_vc = pose_handler_->GetPositionMeasurement();
            q_cv = pose_handler_->GetAttitudeMeasurement();

            MSF_INFO_STREAM(
                    "initial measurement pos:["
                    <<p_vc.transpose()
                    <<"] orientation: "
                    <<STREAMQUAT(q_cv));

            // Check if we have already input from the measurement sensor.
            if (p_vc.norm() == 0)
                MSF_WARN_STREAM(
                        "No measurements received yet\
                        to initialize position - using [0 0 0]");
            if (q_cv.w() == 1)
                MSF_WARN_STREAM(
                        "No measurements received yet\
                        to initialize attitude - using [1 0 0 0]");

            // init params
            p_ic[0] = 0.0;
            p_ic[1] = 0.0;
            p_ic[2] = 0.0;

            //7.3336e-14 0.707107 -0.707107 7.3123e-14
            q_ic.w() = _q_ic.w();
            q_ic.x() = _q_ic.x();
            q_ic.y() = _q_ic.y();
            q_ic.z() = _q_ic.z();
            q_ic.normalize();

            // Calculate initial attitude and position 
            // based on sensor measurements.
            if (!pose_handler_->ReceivedFirstMeasurement()) {  
                // If there is no pose measurement, only apply q_wv.
                q = q_wv;
            } else {  
                // If there is a pose measurement, 
                // apply q_ic and q_wv to get initial attitude.
                q = (q_ic * q_cv.conjugate() * q_wv).conjugate();
            }

            q.normalize();
            p = p_wv + q_wv.conjugate().toRotationMatrix() * p_vc / scale
                - q.toRotationMatrix() * p_ic;

            a_m = q.inverse() * g;			/// Initial acceleration.

            // Prepare init "measurement"
            // True means that this message contains 
            // initial sensor readings.
            shared_ptr < msf_core::MSF_InitMeasurement<EKFState_T> >
                meas(new msf_core::MSF_InitMeasurement<EKFState_T>(true));

            meas->SetStateInitValue < StateDefinition_T::p > (p);
            meas->SetStateInitValue < StateDefinition_T::v > (v);
            meas->SetStateInitValue < StateDefinition_T::q > (q);
            meas->SetStateInitValue < StateDefinition_T::b_w > (b_w);
            meas->SetStateInitValue < StateDefinition_T::b_a > (b_a);
            meas->SetStateInitValue < StateDefinition_T::L
                > (Eigen::Matrix<double, 1, 1>::Constant(scale));
            meas->SetStateInitValue < StateDefinition_T::q_wv > (q_wv);
            meas->SetStateInitValue < StateDefinition_T::p_wv > (p_wv);
            meas->SetStateInitValue < StateDefinition_T::q_ic > (q_ic);
            meas->SetStateInitValue < StateDefinition_T::p_ic > (p_ic);

            // Call my set P function.
            SetStateCovariance(meas->GetStateCovariance());  
            meas->Getw_m() = w_m;
            meas->Geta_m() = a_m;

            // TODO:: set meas time without ros
            meas->time = msf_timing::GetTimeNow();
            MSF_INFO_STREAM("Initial measurement timestamp: " 
                    << meas->time);

            // Call initialization in core.
            msf_core_->Init(meas);

        }

        // Prior to this call, all states are initialized to zero/identity.
        void ResetState(EKFState_T& state) const {
            //set scale to 1
            Eigen::Matrix<double, 1, 1> scale;
            scale << 1.0;
            state.Set < StateDefinition_T::L > (scale);
        }
        void InitState(EKFState_T& state) const {
            UNUSED(state);
        }

        void CalculateQAuxiliaryStates(EKFState_T& state, double dt) const {
            const msf_core::Vector3 nqwvv = msf_core::Vector3::Constant(
                    config_.pose_noise_q_wv);
            const msf_core::Vector3 npwvv = msf_core::Vector3::Constant(
                    config_.pose_noise_p_wv);
            const msf_core::Vector3 nqicv = msf_core::Vector3::Constant(
                    config_.pose_noise_q_ic);
            const msf_core::Vector3 npicv = msf_core::Vector3::Constant(
                    config_.pose_noise_p_ic);
            const msf_core::Vector1 n_L = msf_core::Vector1::Constant(
                    config_.pose_noise_scale);

            // Compute the blockwise Q values and store them with the states,
            // these then get copied by the core to the correct places in Qd.
            state.GetQBlock<StateDefinition_T::L>() = (dt * n_L.cwiseProduct(n_L))
                .asDiagonal();
            state.GetQBlock<StateDefinition_T::q_wv>() =
                (dt * nqwvv.cwiseProduct(nqwvv)).asDiagonal();
            state.GetQBlock<StateDefinition_T::p_wv>() =
                (dt * npwvv.cwiseProduct(npwvv)).asDiagonal();
            state.GetQBlock<StateDefinition_T::q_ic>() =
                (dt * nqicv.cwiseProduct(nqicv)).asDiagonal();
            state.GetQBlock<StateDefinition_T::p_ic>() =
                (dt * npicv.cwiseProduct(npicv)).asDiagonal();
        }

        void SetStateCovariance(
                Eigen::Matrix<double, EKFState_T::nErrorStatesAtCompileTime,
                EKFState_T::nErrorStatesAtCompileTime>& P) const {
            UNUSED(P);
            // Nothing, we only use the simulated cov 
            // for the core plus diagonal for the rest.
        }

        void AugmentCorrectionVector(
                Eigen::Matrix<double, EKFState_T::nErrorStatesAtCompileTime, 1>& correction) const {
            UNUSED(correction);
        }

        void SanityCheckCorrection(
                EKFState_T& delaystate,
                const EKFState_T& buffstate,
                Eigen::Matrix<double, EKFState_T::nErrorStatesAtCompileTime, 1>& correction) const {
            UNUSED(buffstate);
            UNUSED(correction);

            const EKFState_T& state = delaystate;
            if (state.Get<StateDefinition_T::L>()(0) < 0) {
                MSF_WARN_STREAM_THROTTLE(
                        1,
                        "Negative scale detected: " << state.Get<StateDefinition_T::L>()(0) << ". Correcting to 0.1");
                Eigen::Matrix<double, 1, 1> L_;
                L_ << 0.1;
                delaystate.Set < StateDefinition_T::L > (L_);
            }
        }


        double noiseAcc;
        double noiseAccbias;
        double noiseGyr;
        double noiseGyrbias;

        /***
         *    * Provide a getter for these parameters, 
         *    this is implemented for a given
         *       * middleware or param file parser.
         *          */
        virtual bool GetParamFixedBias() const { return false; }
        virtual double GetParamNoiseAcc() const { return noiseAcc; }
        virtual double GetParamNoiseAccbias() const { return noiseAccbias; }
        virtual double GetParamNoiseGyr() const { return noiseGyr; }
        virtual double GetParamNoiseGyrbias() const { return noiseGyrbias; }
        virtual double GetParamFuzzyTrackingThreshold() const 
        { return 0.001; }

    };

}
#endif // POSE_MEASUREMENTMANAGER_H
