module.exports = {
    applyEnums: function(context, enums, label, idsLabel) {
        context[label] = enums;
        if (!idsLabel) idsLabel = `${label.slice(0, -1)}_IDS`;
        context[idsLabel] = Object.keys(enums).reduce((obj, key) => {
            obj[enums[key]] = key;
            return obj;
        }, {});
    }
};
