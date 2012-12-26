/**
 * @class Ext.ux.layout.CardHistoryLayout
 * @extends Ext.layout.CardLayout
 *
 * This layout extends Ext.layout.CardLayout introducing item transitions history with appropriate methods.
 * It can handle state of toolbar the back control. Also, it provides ability to setup default transition
 * animation via defaultAnimation config option. It remembers each animation used to step forth and provides
 * correct reverse animation for each step back.
 *
 * usage:
 *
 *             var panel = new Ext.Panel({
 *              fullscreen: true,
 *              layout: {
 *                  xtype: 'layout',
 *                  type:'cardhistory',
 *                  defaultAnimation: 'slide',
 *
 *                  // To obtain current card title
 *                  getTitle: function() {
 *                      return Ext.getCmp('someToolbar').titleEl.getHTML();
 *                  },
 *
 *                  // To apply title value
 *                  setTitle: function(text) {
 *                      var tb = Ext.getCmp('someToolbar');
 *                      tb.setTitle(text);
 *                      tb.doComponentLayout();
 *                  },
 *
 *                  // To set Back control state
 *                  setBack: function(visible, text) {
 *                      var backBtn = Ext.getCmp('someBackButton');
 *                      backBtn.setText(text);
 *                      backBtn.setVisible(visible);
 *                      backBtn.doComponentLayout();
 *                  }
 *               },
 *               items: [
 *                   {
 *                       xtype: 'panel',
 *                       // To define card title in a static way
 *                       cardHistoryTitle: 'First list'
 *                   },
 *                   {
 *                       xtype: 'panel',
 *                       cardHistoryTitle: 'Second list'
 *                   }
 *               ]
 *             });
 *
 *
 *  // Using of optionalItemTitle parameter here is one more way to setup card title dynamically
 *  panel.getLayout().forth('someItemId_orInstance', someOptionalAnimation_asName_orInstance, 'optionalItemTitle');
 *  panel.getLayout().back();
 *
 *  There are two ways to define next card title while transiting forth
 *  (in order of precedence, from high to low):
 *
 *    1. Using nextTitle parameter of the forth method
 *    2. Using cardHistoryTitle custom config option for the card
 *
 */

if (!Ext.ux.layout) Ext.ux.layout = {};

Ext.ux.layout.CardHistoryLayout = Ext.extend(Ext.layout.CardLayout, {
    type: 'cardhistory',

    defaultAnimation: false,

    constructor: function(config){
        Ext.apply(this, config);
        Ext.ux.layout.CardHistoryLayout.superclass.constructor.apply(this, arguments);

        this.history = [];

        this.reverse = function(animation) {
            var a = ('string' == typeof animation) ? Ext.anims[animation] : animation;
            if (!a) return animation;

            function BackAnimation(){}
            BackAnimation.prototype = a;
            var ba = new BackAnimation();

            ba.config = Ext.apply({}, {reverse: true}, a.config);
            return ba;
        };
    },

    /**
     * Return true if item history is empty.
     * @returns {boolean}
     */
    isHistoryEmpty: function() {
        return (0 == this.history.length);
    },

    /**
     * Set new active item. Current active item is pushed to the history.
     * @param {Mixed} nextItem The item to set as active.
     * @param {Mixed} animation Animation to use in transition.
     * @param {string} nextTitle Text to set as title after transition.
     */
    forth: function(nextItem, animation, nextTitle) {
        var anim = animation || this.defaultAnimation;
        if ('string' == typeof anim) {
            anim = Ext.anims[anim];
        }
        nextTitle = nextTitle || Ext.getCmp(nextItem)['cardHistoryTitle'] || null;
        var thisItem = this.getActiveItem();
        var thisTitle = this.getTitle() || thisItem['cardHistoryTitle'] || null;

        this.history.push({item: thisItem, title: thisTitle, animation: anim});

        this.setActiveItem(nextItem, anim);
        if (nextTitle) this.setTitle(nextTitle);
        this.setBack(true /*in case of forth transition it is always true*/, thisTitle);
    },

    /**
     * Set active previous item from the history.
     */
    back: function() {
        if (this.isHistoryEmpty()) return;
        var prevState = this.history.pop();
        var prevPrevState = this.isHistoryEmpty() ? null : this.history[this.history.length-1];

        this.setActiveItem(prevState.item, this.reverse(prevState.animation));
        if (prevState.title) this.setTitle(prevState.title);
        this.setBack(!this.isHistoryEmpty(), prevPrevState ? prevPrevState.title : null);
    },

    /**
     * Handler function to set back control visibility and text on each step forth.
     * @param {boolean} visible Is true to set back control visible, flase for hidden.
     * @param {string} text Text to show in the back control.
     */
    setBack: Ext.emptyFn,

    /**
     * Handler function to get current title value.
     */
    getTitle: Ext.emptyFn,

    /**
     * Handler function to set back control visibility and text on each step forth.
     * @param {string} text Text to use as title.
     */
    setTitle: Ext.emptyFn

});

Ext.regLayout('cardhistory', Ext.ux.layout.CardHistoryLayout);