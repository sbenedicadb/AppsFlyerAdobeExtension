/* **************************************************************************
 *
 * ADOBE CONFIDENTIAL
 * ___________________
 *
 * Copyright 2018 Adobe Systems Incorporated
 * All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Adobe Systems Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Adobe Systems Incorporated and its
 * suppliers and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Adobe Systems Incorporated.
 **************************************************************************/

#ifndef ADOBEMOBILE_CORE_MODULEEVENTPROCESSORBASE_H
#define ADOBEMOBILE_CORE_MODULEEVENTPROCESSORBASE_H

#include "Common.h"
#include "Object.h"

namespace AdobeMarketingMobile {
    class Event;
    class Module;

    /**
     * Base class for all event procesors. In most cases, subclasses should derive from
     * ModuleEventProcessor<TModule> instead of deriving from this class directly.
     */
    class ModuleEventProcessorBase : public Object {
        friend class Module; // Module should only access private methods marked for use by Module.
        friend class ModuleEventProcessorMethods; // For testing

    public:
        virtual std::shared_ptr<Event> Process(const std::shared_ptr<Event>& event) = 0;

        /**
         * @returns the parent module or nullptr if the parent module was destructed or unregistered.
         *
         * NOTE: In rare cases where this function is called concurrently with UnregisterModule, it is
         * possible for GetParentModule() to return a non-nullptr to an unregistered module.
         */
        std::shared_ptr<Module> GetParentModule();

        /**
         * @return true once this is registered and OnRegistered() is called.
         */
        ADOBE_VIRTUAL_FOR_TESTS bool IsFullyRegistered();

        /**
         * @return true once this is unregistered and OnUnregistered() is called.
         */
        ADOBE_VIRTUAL_FOR_TESTS bool IsFullyUnregistered();

    protected:
        /**
         * Constructor.
         * For use by subclasses.
         */
        ModuleEventProcessorBase();

        /**
         * This method is invoked shortly after the processor is registered.
         *
         * Subclasses should override this method to perform initialization tasks. Subclasses should
         * minimize the amount of initialization performed in the constructor and should instead prefer to
         * perform initialization in OnRegistered().
         *
         * Threading
         * =========
         * @see Module for threading guarantees.
         *
         * Ordering
         * ========
         * This method is guaranteed to be called once, asynchronously, after Module::RegisterProcessor<TProcessor>()
         * is called. This call is guaranteed to occur after Module::OnRegistered.
         */
        virtual void OnRegistered();

        /**
         * This method is invoked shortly before the processor is fully unregistered.
         *
         * Subclasses should override this method to perform cleanup tasks. Subclasses should
         * minimize the amount of cleanup performed in the destructor and should instead prefer to
         * perform cleanup in OnUnregistered().
         *
         * Threading
         * =========
         * @see Module for threading guarantees.
         *
         * Ordering
         * ========
         * This method is guaranteed to be called once, asynchronously, after Module::UnregisterModule()
         * is called. This call is guaranteed to occur before Module::OnUnregistered.
         */
        virtual void OnUnregistered();

    private:
        /**
         * @private
         * Called during creation to finish 2-phase initialization of this.
         * For use by Module only.
         */
        ADOBE_VIRTUAL_FOR_TESTS void Init(const std::shared_ptr<Module>& parent_module);

        ADOBE_VIRTUAL_FOR_TESTS std::shared_ptr<Module> GetParentModuleImpl();

        /**
         * @private
         * Calls OnRegistered.
         *
         * For use by Module only.
         */
        ADOBE_VIRTUAL_FOR_TESTS void CallOnRegistered();

        /**
         * @private
         * Calls OnUnregistered.
         *
         * For use by Module only.
         */
        ADOBE_VIRTUAL_FOR_TESTS void CallOnUnregistered();

        /**
         * @private
         * the parent module
         */
        std::weak_ptr<Module> parent_module_;

        /**
         * @private
         *
         * Set to true once this is fully registered (OnRegistered called).
         */
        std::atomic<bool> is_fully_registered_;

        /**
         * @private
         *
         * Set to true once this is fully unregistered (OnUnregistered called).
         */
        std::atomic<bool> is_fully_unregistered_;
    };
}

#endif //ADOBESDK_CORE_MODULEEVENTPROCESSORBASE_H
