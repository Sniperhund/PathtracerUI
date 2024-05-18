//
// Created by Lucas Täkker on 18/05/2024.
//

#pragma once

namespace UI {
    class Layer {
    public:
        void InternalStart() {

        }
        void InternalUpdate() {

        }

    private:
        virtual void Start() = 0;
        virtual void Update() = 0;
    };
} // UI