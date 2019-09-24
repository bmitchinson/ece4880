import { h, Component } from 'preact';
import { TextField, Button, Radio } from 'preact-fluid';
import { database } from '../../firebase/firebase';

import style from './form.scss';

const buttonRef = database.collection('toggles').doc('button');

interface MyProps {
    userIsNotAuth?: boolean;
}
interface MyState {
    submitEnable: boolean;
    validations: {
        phoneNumError: string;
        highTempThreshError: string;
        lowTempThreshError: string;
    };
    inputs: {
        phoneNumInput: string;
        highTempMsgInput: string;
        lowTempMsgInput: string;
        highTempThreshInput: string;
        lowTempThreshInput: string;
    };
    buttonIsPressed: boolean;
}

const theme = {
    primaryColor: '#90d7c2',
    linkColor: '#fff'
};

export default class Form extends Component<MyProps, MyState> {
    constructor(props) {
        super(props);
        this.state = {
            submitEnable: false,
            validations: {
                phoneNumError: '',
                highTempThreshError: '',
                lowTempThreshError: ''
            },
            inputs: {
                phoneNumInput: '',
                highTempMsgInput: '',
                lowTempMsgInput: '',
                highTempThreshInput: '',
                lowTempThreshInput: ''
            },
            buttonIsPressed: false
        };
    }

    componentDidMount() {
        buttonRef.onSnapshot(doc => {
            const buttonIsPressed = doc.data().buttonIsPressed;
            this.setState(prev => ({ ...prev, buttonIsPressed }));
        });
        buttonRef.onSnapshot(doc => {
            const buttonIsPressed = doc.data().isPressed;
            this.setState({ buttonIsPressed });
        });
    }

    enableButtonIfAllValid() {
        const {
            phoneNumError,
            highTempThreshError,
            lowTempThreshError
        } = this.state.validations;
        const {
            phoneNumInput,
            highTempMsgInput,
            lowTempMsgInput,
            highTempThreshInput,
            lowTempThreshInput
        } = this.state.inputs;

        if (
            !phoneNumError &&
            !highTempThreshError &&
            !lowTempThreshError &&
            phoneNumInput &&
            highTempMsgInput &&
            lowTempMsgInput &&
            highTempThreshInput &&
            lowTempThreshInput
        ) {
            this.setState({ submitEnable: true });
        } else {
            this.setState({ submitEnable: false });
        }
    }

    toggleLcd = () => {
        this.setState(
            { buttonIsPressed: !this.state.buttonIsPressed },
            this.relayButtonIsPressted
        );
    };

    relayButtonIsPressted = () => {
        buttonRef.set({
            isPressed: this.state.buttonIsPressed
        });
    };

    handlePhone = (e: any) => {
        const text: string = e.target.value;
        this.setState(
            prev => ({
                inputs: { ...prev.inputs, phoneNumInput: text }
            }),
            () => this.validatePhone()
        );
    };

    handleHighMsg = (e: any) => {
        const text: string = e.target.value;
        this.setState(
            prev => ({
                inputs: { ...prev.inputs, highTempMsgInput: text }
            }),
            () => this.enableButtonIfAllValid()
        );
    };

    handleLowMsg = (e: any) => {
        const text: string = e.target.value;
        this.setState(
            prev => ({
                inputs: { ...prev.inputs, lowTempMsgInput: text }
            }),
            () => this.enableButtonIfAllValid()
        );
    };

    handleHighThresh = (e: any) => {
        const text: string = e.target.value;
        this.setState(
            prev => ({
                inputs: { ...prev.inputs, highTempThreshInput: text }
            }),
            () => this.validateTemps()
        );
    };

    handleLowThresh = (e: any) => {
        const text: string = e.target.value;
        this.setState(
            prev => ({
                inputs: { ...prev.inputs, lowTempThreshInput: text }
            }),
            () => this.validateTemps()
        );
    };

    validatePhone = () => {
        const phoneText: string = this.state.inputs.phoneNumInput;
        const regEx = /^\(?([0-9]{3})\)?[-]?([0-9]{3})[-]?([0-9]{4})$/;
        const errorMsg: string =
            !phoneText || phoneText.match(regEx) ? '' : '###-###-####';
        this.setState(
            prev => ({
                validations: {
                    ...prev.validations,
                    phoneNumError: errorMsg
                }
            }),
            () => this.enableButtonIfAllValid()
        );
    };

    validateTemps = () => {
        const { lowTempThreshInput, highTempThreshInput } = this.state.inputs;
        const lowTempError =
            !lowTempThreshInput ||
            (parseInt(lowTempThreshInput) >= -15 &&
                parseInt(lowTempThreshInput) <= 50 &&
                !lowTempThreshInput.match(/[^0-9\-]/))
                ? ''
                : 'Enter # between -10 and 50';
        const highTempError =
            !highTempThreshInput ||
            (parseInt(highTempThreshInput) >= -15 &&
                parseInt(highTempThreshInput) <= 50 &&
                !highTempThreshInput.match(/[^0-9\-]/))
                ? ''
                : 'Enter # between -10 and 50';
        this.setState(
            prev => ({
                validations: {
                    ...prev.validations,
                    lowTempThreshError: lowTempError,
                    highTempThreshError: highTempError
                }
            }),
            () => this.enableButtonIfAllValid()
        );
    };

    handleSubmit() {
        console.log('*Sent off to google*');
    }

    render() {
        const userIsNotAuth = this.props.userIsNotAuth;
        // const submitEnable = userIsAuth && this.state.submitEnable;
        const submitEnable = this.state.submitEnable;
        const gridPref = { columns: '1fr' };
        const {
            phoneNumError,
            highTempThreshError,
            lowTempThreshError
        } = this.state.validations;
        return (
            <div class={style.FormContainer}>
                <h2>Notification Controls:</h2>
                <div class={style.FieldsContainer}>
                    <TextField
                        className={style.InputPad}
                        hideLabel
                        errorText={phoneNumError}
                        placeholder="Phone #"
                        grid={gridPref}
                        onChange={this.handlePhone}
                        value={this.state.inputs.phoneNumInput}
                        disabled={userIsNotAuth}
                    />
                    <TextField
                        className={style.InputPad}
                        hideLabel
                        placeholder="High Temp Alert Message"
                        grid={gridPref}
                        onChange={this.handleHighMsg}
                        value={this.state.inputs.highTempMsgInput}
                        disabled={userIsNotAuth}
                    />
                    <TextField
                        className={style.InputPad}
                        hideLabel
                        placeholder="Low Temp Alert Message"
                        grid={gridPref}
                        onChange={this.handleLowMsg}
                        value={this.state.inputs.lowTempMsgInput}
                        disabled={userIsNotAuth}
                    />
                    <TextField
                        className={style.InputPad}
                        errorText={highTempThreshError}
                        hideLabel
                        placeholder="High Temp Threshold"
                        grid={gridPref}
                        onChange={this.handleHighThresh}
                        value={this.state.inputs.highTempThreshInput}
                        disabled={userIsNotAuth}
                    />
                    <TextField
                        className={style.InputPad}
                        errorText={lowTempThreshError}
                        hideLabel
                        placeholder="Low Temp Threshold"
                        grid={gridPref}
                        onChange={this.handleLowThresh}
                        value={this.state.inputs.lowTempThreshInput}
                        disabled={userIsNotAuth}
                    />
                    <div className={style.BottomRow}>
                        <Radio
                            className={style.InputPad}
                            checked={this.state.buttonIsPressed}
                            value="lcdChecked"
                            label="LCD On"
                            bgColor="#90d7c2"
                            effect="circle"
                            onChange={this.toggleLcd}
                        />
                        <Button
                            className={style.ButtonPad}
                            onClick={this.handleSubmit}
                            disabled={!submitEnable}
                            theme={theme}
                            size={'large'}
                            primary
                        >
                            Submit
                        </Button>
                    </div>
                </div>
            </div>
        );
    }
}
