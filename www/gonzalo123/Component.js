sap.ui.define(['jquery.sap.global', 'sap/ui/core/UIComponent'],
    function (jQuery, UIComponent) {
        "use strict";

        var Component = UIComponent.extend("gonzalo123.Component", {
            metadata: {
                rootView: "gonzalo123.view.App",
                routing: {
                    config: {
                        targetsClass: "sap.m.routing.Targets",
                        viewPath: "gonzalo123.view",
                        controlId: "rootControl",
                        controlAggregation: "pages",
                        viewType: "XML"
                    },
                    targets: {
                        main: {
                            viewName: "Main",
                            viewLevel: 0
                        }
                    }
                }
            },

            init: function () {
                UIComponent.prototype.init.apply(this, arguments);
                this.getTargets().display("main");
            }
        });

        return Component;
    });
